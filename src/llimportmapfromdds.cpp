#include "../include/llimportmapfromdds.h"
#include "../include/llmaplist.h"


// Public crnlib header.
#include "../externals/crunch/inc/crnlib.h"

// CRN transcoder library.
#define CRND_HEADER_FILE_ONLY
#include "../externals/crunch/inc/crn_decomp.h"
// .DDS file format definitions.
#include "../externals/crunch/inc/dds_defs.h"

using namespace crnlib;

// Loads an entire file into an allocated memory block.
static crn_uint8 *read_file_into_buffer(const char *pFilename, crn_uint32 &size)
{
	size = 0;

	FILE* pFile = NULL;
	fopen_s(&pFile, pFilename, "rb");
	if (!pFile)
		return NULL;

	fseek(pFile, 0, SEEK_END);
	size = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	crn_uint8 *pSrc_file_data = static_cast<crn_uint8*>(malloc(std::max(1U, size)));
	if ((!pSrc_file_data) || (fread(pSrc_file_data, size, 1, pFile) != 1))
	{
		fclose(pFile);
		free(pSrc_file_data);
		size = 0;
		return NULL;
	}

	fclose(pFile);
	return pSrc_file_data;
}

llImportMapFromDDS::llImportMapFromDDS() : llWorker() {
	SetCommandName("ImportMapFromDDS");
}

int llImportMapFromDDS::Prepare(void) {
	if (!llWorker::Prepare()) return 0;

	mapname  = NULL;
	filename = NULL;
	z        = 1.0;
	even     = 0;
	rgb      = 1;

	return 1;
}

int llImportMapFromDDS::RegisterOptions(void) {
	if (!llWorker::RegisterOptions()) return 0;

	RegisterValue("-x1",       &x1);
	RegisterValue("-y1",       &y1);
	RegisterValue("-x2",       &x2);
	RegisterValue("-y2",       &y2);
	RegisterValue("-zscale",   &z);
	RegisterValue("-name",     &mapname);
	RegisterValue("-filename", &filename, LLWORKER_OBL_OPTION);

	RegisterFlag ("-even",     &even);
	//RegisterFlag ("-rgb",      &rgb);

	return 1;
}

int llImportMapFromDDS::Exec(void) {
	if (!llWorker::Exec()) return 0;

	llMap *newmap = _llMapList()->GetMap(mapname);
	if (newmap) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: map %s existing", command_name, mapname);
		return 0;
	}

	crn_uint32 src_file_size;
	crn_uint8 *pSrc_file_data = read_file_into_buffer(filename, src_file_size);
	if (!pSrc_file_data) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "Unable to read DDS file \"%s\"", filename);
		return 0;
	}

	crn_texture_desc tex_desc;
	crn_uint32 *pImages[cCRNMaxFaces * cCRNMaxLevels];
	if (!crn_decompress_dds_to_images(pSrc_file_data, src_file_size, pImages, tex_desc)) {
		free(pSrc_file_data);
		_llLogger()->WriteNextLine(-LOG_ERROR, "Failed unpacking DDS file!");
		return 0;
	}

	_llLogger()->WriteNextLine(-LOG_INFO, "Decompressed texture Dimensions: %ux%u, Faces: %u, Levels: %u, FourCC: 0x%08X '%c' '%c' '%c' '%c'",
		tex_desc.m_width, tex_desc.m_height, tex_desc.m_faces, tex_desc.m_levels, tex_desc.m_fmt_fourcc, 
		std::max(32U, tex_desc.m_fmt_fourcc & 0xFF), 
		std::max(32U, (tex_desc.m_fmt_fourcc >> 8) & 0xFF), 
		std::max(32U, (tex_desc.m_fmt_fourcc >> 16) & 0xFF), 
		std::max(32U, (tex_desc.m_fmt_fourcc >> 24) & 0xFF));

	//use only one level/face:
	crn_uint32 face_index  = 0; 
	crn_uint32 level_index = 0;

	int widthx = std::max(1U, tex_desc.m_width  >> level_index);
    int widthy = std::max(1U, tex_desc.m_height >> level_index);

	if (!Used("-x1")) x1 = _llUtils()->x00;
	if (!Used("-x2")) x2 = _llUtils()->x11;
	if (!Used("-y1")) y1 = _llUtils()->y00;
	if (!Used("-y2")) y2 = _llUtils()->y11;

	llMap * heightmap = NULL;
	if (Used("-rgb"))
		heightmap = new llMap(widthx, widthy, MAP_COLOR);
	else
		heightmap = new llMap(widthx, widthy);
	if (even) heightmap->SetEven();
	heightmap->SetCoordSystem(x1, y1, x2, y2, z);

	unsigned char r, g, b, a;

	//--> see also importmap.cpp
	/* Read the image */
	for (int y=widthy; y>=0; y--) {
		for (int x=0; x<widthx; x++) {

			r = unsigned char( *((pImages[level_index + face_index * tex_desc.m_levels]) + x + widthx*y) & 0xff);
			g = unsigned char((*((pImages[level_index + face_index * tex_desc.m_levels]) + x + widthx*y) & 0xff00)     >> 8);
			b = unsigned char((*((pImages[level_index + face_index * tex_desc.m_levels]) + x + widthx*y) & 0xff0000)   >> 16);
			a = unsigned char((*((pImages[level_index + face_index * tex_desc.m_levels]) + x + widthx*y) & 0xff000000) >> 24);
			
			heightmap->SetTupel(x, y, b, g, r, a);
		}
	}

	_llUtils()->x00 = x1;
	_llUtils()->y00 = y1;
	_llUtils()->x11 = x2;
	_llUtils()->y11 = y2;

	llQuadList     *quads      = heightmap->GenerateQuadList();
	llPointList    *points     = new llPointList(0, quads); 
	llPolygonList  *polygons   = new llPolygonList(points, heightmap);
	llLineList     *lines      = new llLineList(0, points, heightmap);
	llTriangleList *triangles  = new llTriangleList(0, points);

	if (!Used("-name"))
		_llMapList()->AddMap("_heightmap", heightmap, points, triangles, polygons, lines);
	else
		_llMapList()->AddMap(mapname, heightmap, points, triangles, polygons, lines);

	return 1;
}
