#include "../include/llreadpolygondatafile.h"

llReadPolygonDataFile::llReadPolygonDataFile() : llSet() {
	SetCommandName("ReadPolygonDataFile");
}

int llReadPolygonDataFile::Prepare(void) {
	if (!llSet::Prepare()) return 0;

	polygon_name = filename = NULL;
	scale_x = scale_y       = 1.;
	trans_x = trans_y       = 0.;

	return 1;
}

int llReadPolygonDataFile::RegisterOptions(void) {
	if (!llSet::RegisterOptions()) return 0;

	RegisterValue("-filename", &filename, LLWORKER_OBL_OPTION);
	RegisterValue("-name",     &polygon_name);
	RegisterValue("-scale_x",  &scale_x);
	RegisterValue("-scale_y",  &scale_y);
	RegisterValue("-transx",   &trans_x);
	RegisterValue("-transy",   &trans_y);
	return 1;
}


int llReadPolygonDataFile::Exec(void) {
	if (!llSet::Exec()) return 0;

	llPolygonList *polygons = _llMapList()->GetPolygonList(mapname);
	if (!polygons) {
		_llLogger()->WriteNextLine(-LOG_FATAL, "%s: no polygon list in map [%s]", command_name, mapname);
		return 0;
	}

	FILE * file;
	if (fopen_s(&file, filename, "r")) {
		_llLogger()->WriteNextLine(-LOG_ERROR,"Unable to open %s", filename);
		return 0;
	} else {
		char line[1000];
		char *linenew;
		char *current_polygon = NULL;
		int num_vertex = 0;
		float x1, y1, x2, y2;
		while (fgets(line,1000,file)) {
			linenew = line;
			_llUtils()->StripSpaces(&linenew);
			if (strlen(linenew)) {
				if (linenew[0] == '[') {
					//new polygon name
					for (unsigned int i=1;i<strlen(linenew);i++) 
						if (linenew[i] == ']') linenew[i] ='\0';
					if (current_polygon) 
						delete [] current_polygon;
					current_polygon = new char[strlen(linenew+1)+1];
					strcpy_s(current_polygon, strlen(linenew+1)+1, linenew+1);
					num_vertex = 0;
				} else if (linenew[0] != '#' && linenew[0] != ';') {
					//read coordinates
					if (!current_polygon) {
						_llLogger()->WriteNextLine(LOG_ERROR,"%s: no polygon section for [%s]", command_name, linenew);
					} else {
						if (!polygon_name || _stricmp(polygon_name, current_polygon)==0) {
							num_vertex++;
							int has_komma = 0;
							for (int i=0;i<strlen(linenew);i++)
								if (linenew[i] == ',') 
									has_komma = 1;
							if (num_vertex == 1) {
								if (has_komma)
									sscanf_s(linenew, "%f,%f", &x1, &y1);
								else
									sscanf_s(linenew, "%f %f", &x1, &y1);
							} else if (num_vertex == 2) {
								if (has_komma)
									sscanf_s(linenew, "%f,%f", &x2, &y2);
								else
									sscanf_s(linenew, "%f %f", &x2, &y2);
								polygons->AddPolygon(x1*scale_x+trans_x, y1*scale_y+trans_y, x2*scale_x+trans_x, y2*scale_y+trans_y, current_polygon);    	    		
							} else {
								if (has_komma)
									sscanf_s(linenew, "%f,%f", &x1, &y1);
								else
									sscanf_s(linenew, "%f %f", &x1, &y1);
								polygons->AddVertexToPolygon(x1*scale_x+trans_x, y1*scale_y+trans_y, current_polygon);    
							}
						}
					}
				}
			}
		} //while
		if (current_polygon) delete [] current_polygon;
	} //fopen

	return 1;

}