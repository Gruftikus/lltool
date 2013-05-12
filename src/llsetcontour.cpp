#include "../include/llsetcontour.h"

llSetContour::llSetContour() : llSet() {
	SetCommandName("ContourLine");
}

int llSetContour::Prepare(void) {
	if (!llSet::Prepare()) return 0;

	findmin = findmax = linear = onlyintracell = setmin = setmax = 0;

	return 1;
}

int llSetContour::RegisterOptions(void) {
	if (!llSet::RegisterOptions()) return 0;

	RegisterValue("-x",       &gridx, LLWORKER_OBL_OPTION);
	RegisterValue("-y",       &gridy, LLWORKER_OBL_OPTION);
	RegisterValue("-z",       &z,     LLWORKER_OBL_OPTION);
	RegisterValue("-offsetx", &offsetx);
	RegisterValue("-offsety", &offsety);

	RegisterFlag("-findmin",       &findmin);
	RegisterFlag("-findmax",       &findmax);
	RegisterFlag("-setmin",        &setmin);
	RegisterFlag("-setmax",        &setmax);
	RegisterFlag("-linear",        &linear);
	RegisterFlag("-onlyintracell", &onlyintracell);

	return 1;
}


int llSetContour::Exec(void) {
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

	if (onlyintracell) {
		if (!cellsize_x) {
			_llLogger()->WriteNextLine(-LOG_WARNING,"%s: _cellsize_x not defined, -onlyintracell disabled", command_name);
			onlyintracell = 0;
		}
		if (!cellsize_y) {
			_llLogger()->WriteNextLine(-LOG_WARNING,"%s: _cellsize_y not defined, -onlyintracell disabled", command_name);
			onlyintracell = 0;
		}
	}
	
	int con_points=0;

	for (float y=_llUtils()->y00+offsety+gridy; y<=(_llUtils()->y11); y+=gridy) {
		int lastflag = 0;
		float minx   = 0;
		float minz   = 999999;
		float maxx   = 0;
		float maxz   = -999999;
		for (float x=_llUtils()->x00+offsetx+gridx; x<=(_llUtils()->x11); x+=gridx) {
			if (lastflag==-1 && findmin) {
				for (float x1=x-gridx; x1<=x; x1++) {
					if (map->GetZ(x1,y) < minz) {
						minz = map->GetZ(x1, y);
						minx = x1;
						if (onlyintracell && int(x1/cellsize_x)*int(cellsize_x)==int(x1)) {
							lastflag = 0;
							minz     = -999999;
						}
					}
				}
			}
			if (lastflag==1 && findmax) {
				for (float x1=x-gridx; x1<=x; x1++) {
					if (map->GetZ(x1,y) > maxz) {
						maxz = map->GetZ(x1, y);
						maxx = x1;
						if (onlyintracell && int(x1/cellsize_x)*int(cellsize_x)==int(x1)) {
							lastflag = 0;
							minz     = -999999;
						}
					}
				}
			}

			if (((map->GetZ(x,y) > z) && (map->GetZ(x-gridx,y) < z))
				|| ((map->GetZ(x,y) < z) && (map->GetZ(x-gridx,y) > z))) {

					//breakline crossing
					float myx = x-gridx*(map->GetZ(x,y)-z)/
						(map->GetZ(x,y)
						- map->GetZ(x-gridx,y));
					if (!linear) {
						myx = 0;
						float numx = 0;
						float jump=map->GetWidthXPerRaw()/3.f;
						for (float x1=x-gridx; x1<=x; x1+=jump) {
							if ((map->GetZ(x1,y) < z && map->GetZ(x1-jump,y) >= z)
								|| (map->GetZ(x1,y) > z && map->GetZ(x1-jump,y) <= z)) {
									myx+=x1;
									numx++;
							}
						}
						if (numx) {
							myx /= numx;
						} else { //can happen if the grid is too small, nothing serious
							myx = x - gridx/2.0f; 
							//_llLogger()->WriteNextLine(LOG_WARNING,"%s: contour not found for x", command_name);
						}
					}
					int flag=1;
					if (map->GetZ(x,y) < map->GetZ(x-gridx,y)) 
						flag=-1;
					if (lastflag==-1 && flag==1 && minz<99998) { 
						if (points->GetMinDistance(minx,y) > minab) { //  && 
							if (!cellsize_x || points->GetMinDistanceGrid(minx,y,cellsize_x,1) > minab) {
								//points->AddPoint(minx,y,heightmap->GetZ(minx,y));	
								points->AddPoint(minx, y, z);
								con_points++;	
								minz = 99999;
							}
						} else points->SetFlag(points->GetClosestPoint(minx,y), flag);
					}
					if (lastflag==1 && flag==-1 && maxz>-99998) { 
						if (points->GetMinDistance(maxx,y) > minab) { //  && 
							if (!cellsize_x || points->GetMinDistanceGrid(maxx,y,cellsize_x,1) > minab) {
								//points->AddPoint(minx,y,heightmap->GetZ(minx,y));	
								points->AddPoint(maxx, y, z);
								con_points++;	
								maxz = -99999;
							}
						} else points->SetFlag(points->GetClosestPoint(maxx,y), flag);
					}
					//float minab_grid=-1;
					float minab_grid = minab;
					if (points->GetMinDistance(myx,y) > minab && (!cellsize_x || points->GetMinDistanceGrid(myx,y,cellsize_x,1) > minab_grid)) {
						int p = points->AddPoint(myx, y, map->GetZ(myx,y));	
						con_points++;
						points->SetFlag(p, flag);
						if (setmin && map->GetZ(x-gridx,y) < z && points->GetMinDistance(x-gridx,y) > minab) {
							points->AddPoint(x-gridx, y, map->GetZ(x-gridx,y)); 
							con_points++;
						}
						if (setmin && map->GetZ(x,y) < z && points->GetMinDistance(x,y) > minab) {
							points->AddPoint(x, y, map->GetZ(x,y)); 
							con_points++;
						}
						if (setmax && map->GetZ(x-gridx,y) > z && points->GetMinDistance(x-gridx,y) > minab) {
							points->AddPoint(x-gridx, y, map->GetZ(x-gridx,y)); 
							con_points++;
						}
						if (setmax && map->GetZ(x,y) > z && points->GetMinDistance(x,y) > minab) {
							points->AddPoint(x, y, map->GetZ(x,y)); 
							con_points++;
						}

					}
					lastflag=flag;
			}
		}
	}

	for (float x=_llUtils()->x00+offsetx+gridx; x<=(_llUtils()->x11); x+=gridx) {
		int   lastflag = 0;
		float miny = 0;
		float minz = 999999;
		float maxy = 0;
		float maxz = -999999;
		for (float y=_llUtils()->y00+offsety+gridy; y<=(_llUtils()->y11-1); y+=gridy) {

			if (lastflag==-1 && findmin) {
				for (float y1=y-gridy; y1<=y; y1++) {
					if (map->GetZ(x,y1) < minz) {
						minz = map->GetZ(x, y1);
						miny = y1;
						if (onlyintracell && int(y1/cellsize_y)*int(cellsize_y)==int(y1)) {
							lastflag = 0;
							minz     = -999999;
						}
					}
				}
			}
			if (lastflag==1 && findmax) {
				for (float y1=y-gridy; y1<=y; y1++) {
					if (map->GetZ(x,y1) > maxz) {
						maxz = map->GetZ(x, y1);
						maxy = y1;
						if (onlyintracell && int(y1/cellsize_y)*int(cellsize_y)==int(y1)) {
							lastflag = 0;
							minz     = -999999;
						}
					}
				}
			}

			if (((map->GetZ(x,y) > z ) && (map->GetZ(x,y-gridy) < z))
				|| ((map->GetZ(x,y) < z ) && (map->GetZ(x,y-gridy) > z))) {
					//breakline crossing
					float myy = y-gridy*(map->GetZ(x,y)-z)/
						(map->GetZ(x,y)
						- map->GetZ(x,y-gridy));
					if (!linear) {
						myy = 0;
						float numy = 0;
						float jump = map->GetWidthYPerRaw()/3.f;
						for (float y1=y-gridy; y1<=y; y1+=jump) {
							if ((map->GetZ(x,y1) < z && map->GetZ(x,y1-jump) >= z)
								|| (map->GetZ(x,y1) > z && map->GetZ(x,y1-jump) <= z)) {
									myy += y1;
									numy++;
							}
						}
						if (numy) {
							myy /= numy;
						} else {
							myy = y - gridy/2.0f;
							//_llLogger()->WriteNextLine(LOG_WARNING,"%s: contour not found for y", command_name);
						}
					}
					int flag = 1;
					if (map->GetZ(x,y) < map->GetZ(x,y-gridy)) 
						flag=-1;
					if (lastflag==-1 && flag==1 && minz<99998) { 
						if (points->GetMinDistance(x,miny) > minab) { 
							if (!cellsize_y || points->GetMinDistanceGrid(x,miny,cellsize_y,2) > minab) {
								//points->AddPoint(x,miny,heightmap->GetZ(x,miny));	
								points->AddPoint(x, miny, z);
								con_points++;	
								minz = 99999;
							}
						} else points->SetFlag(points->GetClosestPoint(x,miny), flag);
					}
					if (lastflag==1 && flag==-1 && maxz>-99998) { 
						if (points->GetMinDistance(x,maxy) > minab) { 
							if (!cellsize_y || points->GetMinDistanceGrid(x,maxy,cellsize_y,2) > minab) {
								//points->AddPoint(x,miny,heightmap->GetZ(x,miny));	
								points->AddPoint(x, maxy, z);
								con_points++;	
								minz = 99999;
							}
						} else points->SetFlag(points->GetClosestPoint(x,maxy), flag);
					}
					//float minab_grid=-1;
					float minab_grid = minab;
					if (points->GetMinDistance(x,myy) > minab && (!cellsize_y || points->GetMinDistanceGrid(x,myy,cellsize_y,2) > minab_grid)) {
						int p = points->AddPoint(x, myy, map->GetZ(x,myy));	
						con_points++;
						points->SetFlag(p, flag);
						if (setmin && map->GetZ(x,y-gridy) < z && points->GetMinDistance(x,y-gridy) > minab) {
							points->AddPoint(x, y-gridy, map->GetZ(x,y-gridy)); 
							con_points++;
						}
						if (setmin && map->GetZ(x,y) < z && points->GetMinDistance(x,y) > minab) {
							points->AddPoint(x, y, map->GetZ(x,y)); 
							con_points++;
						}
						if (setmax && map->GetZ(x,y-gridy)>z && points->GetMinDistance(x,y-gridy) > minab) {
							points->AddPoint(x, y-gridy, map->GetZ(x,y-gridy)); 
							con_points++;
						}
						if (setmax && map->GetZ(x,y) > z && points->GetMinDistance(x,y) > minab) {
							points->AddPoint(x, y, map->GetZ(x,y)); 
							con_points++;
						}

					}
					lastflag=flag;
			}
		}
	}

	_llLogger()->WriteNextLine(-LOG_COMMAND,"%s: %i vertices set", command_name, con_points);


	return 1;
}