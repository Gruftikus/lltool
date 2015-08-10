#include "../include/llsetgrid.h"

llSetGrid::llSetGrid() : llSet() {
	SetCommandName("SetGrid");

	stepx = stepy = -1;
}

int llSetGrid::RegisterOptions(void) {
	if (!llSet::RegisterOptions()) return 0;

	RegisterValue("-x", &gridx, LLWORKER_OBL_OPTION);
	RegisterValue("-y", &gridy, LLWORKER_OBL_OPTION);
	RegisterValue("-stepx", &stepx);
	RegisterValue("-stepy", &stepy);

	return 1;
}


int llSetGrid::Exec(void) {
	if (!llSet::Exec()) return 0;

	float minab = 0;
	if (_llUtils()->GetValueF("_mindistance"))
		minab = (float)(*_llUtils()->GetValueF("_mindistance"));

	if (!Used("-stepx")) stepx = -1;
	if (!Used("-stepy")) stepy = -1;

	for (float x=floor(_llUtils()->x00/gridx)*gridx; x<=(_llUtils()->x11+1); x+=gridx) {
		for (float y=floor(_llUtils()->y00/gridy)*gridy; y<=(_llUtils()->y11+1); y+=gridy) {
			float x1=x, y1=y;
			if (x >= _llUtils()->x11) x1 = _llUtils()->x11;
			if (y >= _llUtils()->y11) y1 = _llUtils()->y11;

			if (map->IsInMap(x1,y1) && points->GetMinDistance(x1,y1) > minab) {
				points->AddPoint(x1, y1, map->GetZ(x1,y1));	
				//gen_npoints++;
			}

			if (stepx > 0) {
				for (float xx=x1; xx<(x1+gridx); xx+=stepx) {
					if (map->IsInMap(xx,y1) && points->GetMinDistance(xx,y1) > minab) {
						points->AddPoint(xx, y1, map->GetZ(xx,y1));	
					}
				}
			}

			if (stepy > 0) {
				for (float yy=y1; yy<(y1+gridy); yy+=stepy) {
					if (map->IsInMap(x1,yy) && points->GetMinDistance(x1,yy) > minab) {
						points->AddPoint(x1, yy, map->GetZ(x1,yy));	
					}
				}
			}
		}
	}

	return 1;
}