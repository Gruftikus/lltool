#include "../include/llpanorama.h"


llPanorama::llPanorama() : llSet() {
	SetCommandName("Panorama");
}

int llPanorama::Prepare(void) {
	if (!llSet::Prepare()) return 0;

	keepout = 0;

	return 1;
}

int llPanorama::RegisterOptions(void) {
	if (!llSet::RegisterOptions()) return 0;

	RegisterValue("-x",       &gx, LLWORKER_OBL_OPTION);
	RegisterValue("-y",       &gy, LLWORKER_OBL_OPTION);
	RegisterValue("-dist",    &minab2);
	RegisterValue("-keepout", &keepout);

	return 1;
}


int llPanorama::Exec(void) {
	if (!llSet::Exec()) return 0;
	float minab = 0;
	if (_llUtils()->GetValueF("_mindistance"))
		minab = (float)(*_llUtils()->GetValueF("_mindistance"));
	float cellsize_x = 0;
	if (_llUtils()->GetValueF("_cellsize_x"))
		cellsize_x = (float)(*_llUtils()->GetValueF("_cellsize_x"));
	float cellsize_y = 0;
	if (_llUtils()->GetValueF("_cellsize_y"))
		cellsize_y = (float)(*_llUtils()->GetValueF("_cellsize_y"));

	if (!cellsize_x) cellsize_x = -minab - 1.0f;
	if (!cellsize_y) cellsize_y = -minab - 1.0f;

	//float minab2   = 4 * minab; 
	if (!Used("-dist")) minab2   = 4 * minab; 
	int pan_points = 0;

	if (!map->IsInMap(gx, gy)) {
		_llLogger()->WriteNextLine(-LOG_ERROR,"%s: Point (%f, %f) not in map", command_name, gx, gy);
		return 0;
	}

	int used_minz = 0;
	float myz = map->GetZ(gx, gy); 
	for (float y=_llUtils()->y00; y<_llUtils()->y11; y+=minab2) {
		float deg = atan((gy - y) / (gx - _llUtils()->x00));
		float minab_x   = minab2*cos(deg);
		if (minab_x < 1.0) minab_x = 1.0;
		float keepout_x = keepout*cos(deg);
		float minab_y   = minab*sin(deg);
		//float keepout_y = keepout*sin(deg);
		float y1        = y;
		float minx=0, miny=0, minz=0;
		used_minz = 0;
		for (float x1=_llUtils()->x00; x1< (gx - keepout_x); x1+=minab_x) {
			float dist  = sqrt(pow(gx-x1,2) + pow(gy-y1,2));
			float distz = map->GetZ(x1, y1) - myz;
			if (distz/dist > minz) {
				minz = distz/dist;
				minx = x1;
				miny = y1;
				used_minz = 1;
			}
			y1 += minab_y;
		}
		if (used_minz) {
			if (points->GetMinDistance(minx,miny) > minab && 
				points->GetMinDistanceGrid(minx,miny,cellsize_x,1) > minab && 
				points->GetMinDistanceGrid(minx,miny,cellsize_y,2) > minab && 
				map->GetZ(minx,miny) > myz) {
					points->AddPoint(minx, miny, map->GetZ(minx,miny));	
					pan_points++;
			}
		}
	} //for y

	for (float y=_llUtils()->y00; y<_llUtils()->y11; y+=minab2) {
		float deg = atan((gy - y)/(gx - _llUtils()->x11));
		float minab_x   = minab2*cos(deg);
		if (minab_x < 1.0) minab_x = 1.0;
		float keepout_x = keepout*cos(deg);
		float minab_y   = minab*sin(deg);
		//float keepout_y = keepout*sin(deg);
		float y1        = y;
		float minx=0, miny=0, minz=0;
		used_minz = 0;
		for (float x1=_llUtils()->x11; x1>(gx + keepout_x); x1-=minab_x) {
			float dist  = sqrt(pow(gx-x1,2) + pow(gy-y1,2));
			float distz = map->GetZ(x1, y1) - myz;
			if (distz/dist > minz) {
				minz = distz/dist;
				minx = x1;
				miny = y1;
				used_minz = 1;
			}
			y1 -= minab_y;
		}
		if (used_minz) {
			if (points->GetMinDistance(minx,miny) > minab && 
				points->GetMinDistanceGrid(minx,miny,cellsize_x,1) > minab && 
				points->GetMinDistanceGrid(minx,miny,cellsize_y,2) > minab && 
				map->GetZ(minx,miny)>myz) {
					points->AddPoint(minx, miny, map->GetZ(minx,miny));	
					pan_points++;
			}
		}
	} //for y

	for (float x=_llUtils()->x00; x<_llUtils()->x11; x+=minab2) {
		float deg = atan((gx - x) / (gy - _llUtils()->y00));
		float minab_y   = minab2*cos(deg);
		if (minab_y < 1.0) minab_y = 1.0;
		float keepout_y = keepout*cos(deg);
		float minab_x   = minab*sin(deg);
		//float keepout_x = keepout*sin(deg);
		float x1        = x;
		float minx=0, miny=0, minz=0;
		used_minz = 0;
		for (float y1=_llUtils()->y00; y1< (gy - keepout_y); y1+=minab_y) {
			float dist  = sqrt(pow(gy-y1,2) + pow(gx-x1,2));
			float distz = map->GetZ(x1,y1) - myz;
			if (distz/dist > minz) {
				minz = distz/dist;
				minx = x1;
				miny = y1;
				used_minz = 1;
			}
			x1 += minab_x;
		}
		if (used_minz) {
			if (points->GetMinDistance(minx,miny) > minab && 
				points->GetMinDistanceGrid(minx,miny,cellsize_x,1) > minab && 
				points->GetMinDistanceGrid(minx,miny,cellsize_y,2) > minab && 
				map->GetZ(minx, miny) > myz) {
					points->AddPoint(minx, miny, map->GetZ(minx,miny));	
					pan_points++;
			}
		} 			
	} //for x

	for (float x=_llUtils()->x00; x<_llUtils()->x11; x+=minab2) {
		float deg = atan((gx - x) / (gy - _llUtils()->y11)); //!
		float minab_y   = minab2*cos(deg);
		if (minab_y < 1.0) minab_y = 1.0;
		float keepout_y = keepout*cos(deg);
		float minab_x   = minab*sin(deg);
		//float keepout_x = keepout*sin(deg);
		float x1        = x;
		float minx=0, miny=0, minz=0;
		used_minz = 0;
		for (float y1=_llUtils()->y11; y1> (gy + keepout_y); y1-=minab_y) { //!
			float dist  = sqrt(pow(gy-y1,2) + pow(gx-x1,2));
			float distz = map->GetZ(x1,y1)-myz;
			if (distz/dist > minz) {
				minz = distz/dist;
				minx = x1;
				miny = y1;
				used_minz = 1;
			}
			x1 -= minab_x;
		}
		if (used_minz) {
			if (points->GetMinDistance(minx,miny) > minab && 
				points->GetMinDistanceGrid(minx,miny,cellsize_x,1) > minab && 
				points->GetMinDistanceGrid(minx,miny,cellsize_y,2) > minab && 
				map->GetZ(minx,miny)>myz) {
					points->AddPoint(minx, miny, map->GetZ(minx,miny));	
					pan_points++;
			}
		} 		
	} //for x

	_llLogger()->WriteNextLine(-LOG_INFO, "%i panorama vertices set", pan_points);

	return 1;
}