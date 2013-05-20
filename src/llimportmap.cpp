#include "../include/llimportmap.h"
#include "../include/llmaplist.h"

llImportMap::llImportMap() : llWorker() {
	SetCommandName("ImportMap");
}

int llImportMap::Prepare(void) {
	if (!llWorker::Prepare()) return 0;

	mapname  = NULL;
	filename = NULL;
	z        = 1.0;
	even     = 0;

	return 1;
}

int llImportMap::RegisterOptions(void) {
	if (!llWorker::RegisterOptions()) return 0;

	RegisterValue("-x1",       &x1);
	RegisterValue("-y1",       &y1);
	RegisterValue("-x2",       &x2);
	RegisterValue("-y2",       &y2);
	RegisterValue("-zscale",   &z);
	RegisterValue("-name",     &mapname);
	RegisterValue("-filename", &filename, LLWORKER_OBL_OPTION);

	RegisterFlag ("-even",     &even);
	RegisterFlag ("-rgb",      &rgb);

	return 1;
}

int llImportMap::Exec(void) {
	if (!llWorker::Exec()) return 0;

	FILE *fptr;

	llMap *newmap = _llMapList()->GetMap(mapname);
	if (newmap) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: map %s existing", command_name, mapname);
		return 0;
	}

	if (fopen_s(&fptr, filename," rb")) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "Unable to open BMP file \"%s\"", filename);
		return 0;
	}

	HEADER header;
	INFOHEADER infoheader;
			
	//COLOURINDEX colourindex[256];

	/* Read and check the header */
	ReadUShort(fptr, &header.type, 0);
	//fprintf(stderr,"ID is: %d, should be %d\n",header.type,'M'*256+'B');
	ReadUInt(fptr, &header.size, 0);
	_llLogger()->WriteNextLine(-LOG_INFO, "File size is %d bytes", header.size);
	ReadUShort(fptr, &header.reserved1, 0);
	ReadUShort(fptr, &header.reserved2, 0);
	ReadUInt(fptr, &header.offset, 0);
	//fprintf(stderr,"Offset to image data is %d bytes\n",header.offset);

	/* Read and check the information header */
	if (fread(&infoheader, sizeof(INFOHEADER), 1, fptr) != 1) {
		_llLogger()->WriteNextLine(-LOG_FATAL, "Failed to read BMP info header");
		return 0;
	}
	_llLogger()->WriteNextLine(-LOG_INFO, "Image size = %d x %d", infoheader.width, infoheader.height);
	_llLogger()->WriteNextLine(-LOG_INFO, "Bits per pixel is %d", infoheader.bits);

	int widthx = infoheader.width, widthy = infoheader.height;

	if (infoheader.bits != 32 && infoheader.bits != 24 && infoheader.bits != 8) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "Only 8-, 24- and 32-bit files are supported");
		return 0;
	}

	int bytesPerLine = infoheader.width;
	if (infoheader.bits == 24) {
		bytesPerLine = infoheader.width * 3;  /* (for 24 bit images) */
		/* round up to a dword boundary */
		//Thanks to 
		//http://www.siggraph.org/education/materials/HyperVis/asp_data/compimag/bmpfile.htm
		//for the hint
		if (bytesPerLine & 0x0003) {
			bytesPerLine |= 0x0003;
			++bytesPerLine;
		}		     
	} else if (infoheader.bits == 32)
		bytesPerLine = infoheader.width * 4;

	llMap * heightmap = NULL;

	if (!Used("-x1")) x1 = _llUtils()->x00;
	if (!Used("-x2")) x2 = _llUtils()->x11;
	if (!Used("-y1")) y1 = _llUtils()->y00;
	if (!Used("-y2")) y2 = _llUtils()->y11;
	
	if (Used("-rgb"))
		heightmap = new llMap(widthx, widthy, MAP_COLOR);
	else
		heightmap = new llMap(widthx, widthy);
	if (even) heightmap->SetEven();
	heightmap->SetCoordSystem(x1, y1, x2, y2, z);

	COLOURINDEX colourindex[256];
	int gotindex = false;

	/* Read the lookup table if there is one */
	for (int i=0; i<255; i++) {
		colourindex[i].r = rand() % 256;
		colourindex[i].g = rand() % 256;
		colourindex[i].b = rand() % 256;
		colourindex[i].junk = rand() % 256;
	}
	if (infoheader.ncolours > 0) {
		for (unsigned int i=0; i<infoheader.ncolours; i++) {
			if (fread(&colourindex[i].b, sizeof(unsigned char), 1, fptr) != 1) {
				_llLogger()->WriteNextLine(-LOG_ERROR, "Image read failed (ncolours)");
				delete heightmap;
				return 0;
			}
			if (fread(&colourindex[i].g, sizeof(unsigned char), 1, fptr) != 1) {
				_llLogger()->WriteNextLine(-LOG_ERROR, "Image read failed (ncolours)");
				delete heightmap;
				return 0;
			}
			if (fread(&colourindex[i].r, sizeof(unsigned char), 1, fptr) != 1) {
				_llLogger()->WriteNextLine(-LOG_ERROR, "Image read failed (ncolours)");
				delete heightmap;
				return 0;
			}
			if (fread(&colourindex[i].junk, sizeof(unsigned char), 1, fptr) != 1) {
				_llLogger()->WriteNextLine(-LOG_ERROR, "Image read failed (ncolours)");
				delete heightmap;
				return 0;
			}
			//fprintf(stderr,"%3d\t%3d\t%3d\t%3d\n",i,
			//colourindex[i].r,colourindex[i].g,colourindex[i].b);
		}
		gotindex = true;
	}

	/* Seek to the start of the image data */
	fseek(fptr, header.offset, SEEK_SET);

	unsigned char grey, r, g, b, a;

	/* Read the image */
	for (int y=infoheader.height-1; y>=0; y--) {
		int my_bytesPerLine = bytesPerLine;
		for (int x=0; x<infoheader.width; x++) {
			
			switch (infoheader.bits) {
			case 1:
				break;
			case 4:
				break;
			case 8:
				my_bytesPerLine--;
				if (fread(&grey, sizeof(unsigned char), 1, fptr) != 1) {
					_llLogger()->WriteNextLine(-LOG_ERROR, "Image read failed");
					delete heightmap;
					return 0;
				}
				if (gotindex) {
					heightmap->SetTupel(x, y, colourindex[grey].b, colourindex[grey].g, colourindex[grey].r, 0);
				} else {
					heightmap->SetElementRaw(x, y, grey);
				}
				break;
			case 24:
				my_bytesPerLine -= 3;
				if (fread(&b, sizeof(unsigned char), 1, fptr) != 1) {
					_llLogger()->WriteNextLine(-LOG_ERROR, "Image read failed");
					delete heightmap;
					return 0;
				}
				if (fread(&g, sizeof(unsigned char), 1, fptr) != 1) {
					_llLogger()->WriteNextLine(-LOG_ERROR, "Image read failed");
					delete heightmap;
					return 0;
				}
				if (fread(&r, sizeof(unsigned char), 1, fptr) != 1) {
					_llLogger()->WriteNextLine(-LOG_ERROR, "Image read failed");
					delete heightmap;
					return 0;
				}
				heightmap->SetTupel(x, y, b, g, r, 0);
				break;
			case 32:
				my_bytesPerLine -= 4;
				if (fread(&b, sizeof(unsigned char), 1, fptr) != 1) {
					_llLogger()->WriteNextLine(-LOG_ERROR, "Image read failed");
					delete heightmap;
					return 0;
				}
				if (fread(&g, sizeof(unsigned char), 1, fptr) != 1) {
					_llLogger()->WriteNextLine(-LOG_ERROR, "Image read failed");
					delete heightmap;
					return 0;
				}
				if (fread(&r, sizeof(unsigned char), 1, fptr) != 1) {
					_llLogger()->WriteNextLine(-LOG_ERROR, "Image read failed");
					delete heightmap;
					return 0;
				}
				if (fread(&a, sizeof(unsigned char), 1, fptr) != 1) {
					_llLogger()->WriteNextLine(-LOG_ERROR, "Image read failed");
					delete heightmap;
					return 0;
				}
				heightmap->SetTupel(x, y, b, g, r, a);
				break;
			}
		} 
		
		while(my_bytesPerLine>0) {
			my_bytesPerLine--;
			unsigned int trunk1 = 0xff;
			fread(&trunk1, 1, 1, fptr);
		}
	} 

	fclose(fptr);

	_llUtils()->x00 = x1;
	_llUtils()->y00 = y1;
	_llUtils()->x11 = x2;
	_llUtils()->y11 = y2;

	llQuadList     *quads      = heightmap->GenerateQuadList();
	llPointList    *points     = new llPointList(0, quads); 
	llPolygonList  *polygons   = new llPolygonList(points, heightmap);
	llTriangleList *triangles  = new llTriangleList(0, points);

	if (!Used("-name"))
		_llMapList()->AddMap("_heightmap", heightmap, points, triangles, polygons);
	else
		_llMapList()->AddMap(mapname, heightmap, points, triangles, polygons);

	return 1;
}
