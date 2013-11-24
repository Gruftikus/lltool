#include "../include/llexportmaptodds.h"
#include "../include/llmaplist.h"

// Public crnlib header.
#include "../externals/crunch/inc/crnlib.h"

// CRN transcoder library.
#include "../externals/crunch/inc/crn_decomp.h"
// .DDS file format definitions.
#include "../externals/crunch/inc/dds_defs.h"

#ifdef _MSC_VER
#pragma warning (disable: 4244) // conversion from 'int' to 'uint8', possible loss of data
#pragma warning (disable: 4100) // unreferenced formal parameter
// windows.h is only needed here for GetSystemInfo().
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "windows.h"
#endif

const int cDefaultCRNQualityLevel = 128;

// CRN/DDS compression callback function.
static crn_bool progress_callback_func(crn_uint32 phase_index, crn_uint32 total_phases, crn_uint32 subphase_index, 
	crn_uint32 total_subphases, void* pUser_data_ptr)
{
	int percentage_complete = (int)(.5f + (phase_index + float(subphase_index) / total_subphases) * 100.0f) / total_phases;
	if (!(percentage_complete % 10) && percentage_complete > 1 && percentage_complete < 99)
		_llLogger()->WriteNextLine(-LOG_INFO, "Processing: %u%%", std::min(100, std::max(0, percentage_complete)));
	return true;
}

//constructor
llExportMapToDDS::llExportMapToDDS() : llMapWorker() {
	SetCommandName("ExportMapToDDS");
}

int llExportMapToDDS::Prepare(void) {
	if (!llMapWorker::Prepare()) return 0;

	filename = NULL;
	bitrate  = 0.0f;
	quality_level    = -1;
	noAdaptiveBlocks = 0;
	makemips   = 0;
	fmt_string = "DXT1";
	flipx = flipy = 0;

	return 1;
}

int llExportMapToDDS::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;

	RegisterValue("-filename", &filename);
	RegisterValue("-scale",    &scale);
	RegisterValue("-bitrate",  &bitrate);
	RegisterValue("-quality",  &quality_level);
	RegisterValue("-format",   &fmt_string);

	RegisterFlag("-noAdaptiveBlocks",  &noAdaptiveBlocks);
	RegisterFlag("-MakeMips",  &makemips);
	RegisterFlag("-FlipX",    &flipx);
	RegisterFlag("-FlipY",    &flipy);
	
	return 1;
}

int llExportMapToDDS::Exec(void) {
	if (!llMapWorker::Exec()) return 0;

	if (!Used("-filename"))
	    filename = (char *)"map.dds";

	//look for _install_dir:
	if (_llUtils()->GetValue("_install_dir")) {
		char *filename_tmp = new char[strlen(filename) + strlen(_llUtils()->GetValue("_install_dir")) + 2];
		sprintf_s(filename_tmp, strlen(filename) + strlen(_llUtils()->GetValue("_install_dir")) + 2, "%s\\%s", 
			_llUtils()->GetValue("_install_dir"), filename);
		//std::cout << _llUtils()->GetValue("_install_dir") << ":" << filename << std::endl;
		filename = filename_tmp;
	}

	int x1 = map->GetRawX(_llUtils()->x00);
	int y1 = map->GetRawY(_llUtils()->y00);
	int x2 = map->GetRawX(_llUtils()->x11);
	int y2 = map->GetRawY(_llUtils()->y11);

	if (!map->IsUneven()) {
		x1 = map->GetRawX(_llUtils()->x00 + map->GetWidthXPerRaw()*.01f);
		y1 = map->GetRawY(_llUtils()->y00 + map->GetWidthYPerRaw()*.01f);
		x2 = map->GetRawX(_llUtils()->x11 - map->GetWidthXPerRaw()*.01f);
		y2 = map->GetRawY(_llUtils()->y11 - map->GetWidthYPerRaw()*.01f);
	}

	//std::cout << x1 << ":" << y1 << ":" << x2 << ":" << y2 << std::endl;

	int width  = x2-x1+1;
	int height = y2-y1+1;

	using namespace crnlib;

	crn_uint32 *pSrc_image = new crn_uint32[width * height];
	
	for (int y=y2; y>=y1; y--) {
		for (int x=x1; x<=x2; x++) {
			
			unsigned char byte1;
			unsigned char byte2;
			unsigned char byte3;
			unsigned char byte4;

			int tupel = 0;

			int xl = x;
			int yl = y;
			if (flipx) xl = x2 - (x - x1);
			if (flipy) yl = y2 - (y - y1);

			if (Used("-scale")) {
				tupel = map->GetTupelScaled(xl, yl, &byte1, &byte2, &byte3, &byte4, scale);			
			} else {
				tupel = map->GetTupel(xl, yl, &byte1, &byte2, &byte3, &byte4);
			}

			if (tupel) {
				pSrc_image[(xl-x1)+(yl-y1)*width] = byte3 | (byte2 << 8) | (byte1 << 16) | (byte4 << 24);
			} else 
				pSrc_image[(xl-x1)+(yl-y1)*width] = 0;
		}
	}

	bool srgb_colorspace = true;
	bool enable_dxt1a = false;
	bool has_alpha_channel = true;

	bool use_adaptive_block_sizes = true;
	if (noAdaptiveBlocks) use_adaptive_block_sizes = false;

	bool output_crn = false;
	crn_format fmt = cCRNFmtDXT1;
	if (!_stricmp(fmt_string, "dxt1a")) {
		enable_dxt1a = true;
		fmt = cCRNFmtDXT1;
	} else  {
		int f;
		for (f = 0; f < cCRNFmtTotal; f++) {
			if (!_stricmp(fmt_string, crn_get_format_stringa(static_cast<crn_format>(f)))) {
				fmt = static_cast<crn_format>(f);
				break;
			}
		}
		if (f == cCRNFmtTotal) {
			_llLogger()->WriteNextLine(-LOG_ERROR, "Unrecognized pixel format: %s", fmt_string);
			return 0;
		}
	}

	bool create_mipmaps = false;
	if (makemips) create_mipmaps = true;
	

	crn_comp_params comp_params;
	comp_params.m_width = width;
	comp_params.m_height = height;
	comp_params.set_flag(cCRNCompFlagPerceptual, srgb_colorspace);
	comp_params.set_flag(cCRNCompFlagDXT1AForTransparency, enable_dxt1a && has_alpha_channel);
	comp_params.set_flag(cCRNCompFlagHierarchical, use_adaptive_block_sizes);
	comp_params.m_file_type = output_crn ? cCRNFileTypeCRN : cCRNFileTypeDDS;
	comp_params.m_format = (fmt != cCRNFmtInvalid) ? fmt : (has_alpha_channel ? cCRNFmtDXT5 : cCRNFmtDXT1);
	comp_params.m_pImages[0][0] = pSrc_image;

	if (bitrate > 0.0f)
		comp_params.m_target_bitrate = bitrate;
	else if (quality_level >= 0)
		comp_params.m_quality_level = quality_level;
	else if (output_crn)
	{
		// Set a default quality level for CRN, otherwise we'll get the default (highest quality) which leads to huge compressed palettes.
		comp_params.m_quality_level = cDefaultCRNQualityLevel;
	}

      // Determine the # of helper threads (in addition to the main thread) to use during compression. NumberOfCPU's-1 is reasonable.
      SYSTEM_INFO g_system_info;
      GetSystemInfo(&g_system_info);  
      int num_helper_threads = std::max<int>(0, (int)g_system_info.dwNumberOfProcessors - 1);
      comp_params.m_num_helper_threads = num_helper_threads;

      comp_params.m_pProgress_func = progress_callback_func;
            
      // Fill in mipmap parameters struct.
      crn_mipmap_params mip_params;
      mip_params.m_gamma_filtering = srgb_colorspace;
      mip_params.m_mode = create_mipmaps ? cCRNMipModeGenerateMips : cCRNMipModeNoMips;

      crn_uint32 actual_quality_level;
      float actual_bitrate;
      crn_uint32 output_file_size;

      _llLogger()->WriteNextLine(-LOG_INFO, "Compressing to %s\n", crn_get_format_stringa(comp_params.m_format));
      
      // Now compress to DDS or CRN.
      void *pOutput_file_data = crn_compress(comp_params, mip_params, output_file_size, &actual_quality_level, &actual_bitrate);
      
      if (!pOutput_file_data) {
         delete pSrc_image;
		 _llLogger()->WriteNextLine(-LOG_ERROR, "Compression failed");
		 return 0;
	  }

	  _llLogger()->WriteNextLine(-LOG_INFO, "Compressed to %u bytes, quality level: %u, effective bitrate: %f\n", output_file_size, 
		  actual_quality_level, actual_bitrate);

	  // Write the output file.
	  _llLogger()->WriteNextLine(-LOG_INFO, "Writing %s file: %s\n", output_crn ? "CRN" : "DDS", filename);
	  FILE *pFile = fopen(filename, "wb");
	  if ((!pFile) || (fwrite(pOutput_file_data, output_file_size, 1, pFile) != 1) || (fclose(pFile) == EOF)) {
		  crn_free_block(pOutput_file_data);
		  delete pSrc_image;
		  _llLogger()->WriteNextLine(-LOG_ERROR, "Failed writing to output file");
		  return 0;
	  }

	  crn_free_block(pOutput_file_data);
	  delete pSrc_image;


	  return 1;
}
