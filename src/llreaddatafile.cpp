#include "../include/llreaddatafile.h"

llReadDataFile::llReadDataFile() : llSet() {
	SetCommandName("ReadDataFile");

	keepdistancegrid = 0;
}

int llReadDataFile::RegisterOptions(void) {
	if (!llSet::RegisterOptions()) return 0;

	RegisterValue("-filename", &filename, LLWORKER_OBL_OPTION);
	RegisterFlag ("-keepdistancegrid", &keepdistancegrid);

	return 1;
}


int llReadDataFile::Exec(void) {
	if (!llSet::Exec()) return 0;

	FILE *fptr_data;

	if (fopen_s(&fptr_data, filename, "r")) {
		_llLogger()->WriteNextLine(-LOG_FATAL, "Unable to open Data file \"%s\"", filename);
	}

	float minab = 0;
	if (_llUtils()->GetValueF("_mindistance"))
		minab = (float)(*_llUtils()->GetValueF("_mindistance"));
	float minab_grid = minab;
	if (_llUtils()->GetValueF("_mindistancegrid"))
		minab_grid = (float)(*_llUtils()->GetValueF("_mindistancegrid"));
	float cellsize_x = 0;
	if (_llUtils()->GetValueF("_cellsize_x"))
		cellsize_x = (float)(*_llUtils()->GetValueF("_cellsize_x"));
	float cellsize_y = 0;
	if (_llUtils()->GetValueF("_cellsize_y"))
		cellsize_y = (float)(*_llUtils()->GetValueF("_cellsize_y"));
	float cellsize_m = cellsize_x > cellsize_y ? cellsize_x : cellsize_y;
	float mingrid_x = minab_grid + 1.0f;
	float mingrid_y = minab_grid + 1.0f;

	char line[1000];
	char *linex = line;
	//size_t size = 1000;
	int lp = 0, lm = 0;
	while (fgets(line, 1000, fptr_data)) {
		_llUtils()->StripSpaces(&linex);
		float x, y;
		if ((strlen(linex)>2) && (linex[0]!=';') && (linex[0]!='#') && (linex[0]!='[')) {
			if (sscanf_s((const char*) line, "%f %f", &x, &y) == 2) {
				if (!map->IsInMap(x,y)) {
					_llLogger()->WriteNextLine(LOG_ERROR, "Point (%.0f,%.0f) not in map", x, y);
				} else {
					if (keepdistancegrid) {
						if (cellsize_x)
							mingrid_x = points->GetMinDistanceGrid(map->GetCoordX(x), map->GetCoordY(y), cellsize_x, 1);
						if (cellsize_y)
							mingrid_y = points->GetMinDistanceGrid(map->GetCoordX(x), map->GetCoordY(y), cellsize_y, 2);
						if (mingrid_x <= minab_grid || mingrid_y <= minab_grid) {
							points->AddPoint(x, y, map->GetZ(x,y));	
							lp++;
						} else {
							lm++;
						}
					} else {
						points->AddPoint(x, y, map->GetZ(x,y));	
						lp++;
					}
				}
			} else 
				_llLogger()->WriteNextLine(LOG_ERROR, "Syntax error in data file \"%s\" in the line: %i", filename, line);
		}
	}
	if (keepdistancegrid && lm) {
		_llLogger()->WriteNextLine(LOG_INFO, "%s: %i vertex points added from data file %s (%i skipped to keep grid distance)", command_name, lp, filename, lm);
	} else {
		_llLogger()->WriteNextLine(LOG_INFO, "%s: %i vertex points added from data file %s", command_name, lp, filename);
	}

	return 1;
}