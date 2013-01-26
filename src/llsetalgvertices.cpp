#include "../include/llsetalgvertices.h"
#include "../include/llalglist.h"


llSetAlgVertices::llSetAlgVertices() : llSet() {
	SetCommandName("SetAlgVertices");
}

int llSetAlgVertices::Prepare(void) {
	if (!llSet::Prepare()) return 0;

	max = 0;
	alg_list = (char *)"_heightmap_alg";

	return 1;
}

int llSetAlgVertices::RegisterOptions(void) {
	if (!llSet::RegisterOptions()) return 0;

	RegisterValue("-n",   &nmax, LLWORKER_OBL_OPTION);
	RegisterValue("-alg", &alg_list);
	RegisterFlag ("-max", &max);

	return 1;
}


int llSetAlgVertices::Exec(void) {
	if (!llSet::Exec()) return 0;

	llAlgCollection *alg = _llAlgList()->GetAlgCollection(alg_list);
	if (!alg) {
		_llLogger()->WriteNextLine(-LOG_FATAL, "%s: alg collection [%s] not found", command_name, alg_list);
		return 0;
	}

	if (alg->GetSize() == 0) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: no algorithm specified", command_name);
		return 0;
	}


	float minab      = (float) _llUtils()->GetValueF("_mindistance");
	float cellsize_x = (float) _llUtils()->GetValueF("_cellsize_x");
	float cellsize_y = (float) _llUtils()->GetValueF("_cellsize_y");
	float cellsize_m = cellsize_x > cellsize_y ? cellsize_x : cellsize_y;

	int points_done = 0;
	double fraction = 1.0;
	float x1 = _llUtils()->x00;
	float x2 = _llUtils()->x11;
	float y1 = _llUtils()->y00;
	float y2 = _llUtils()->y11;

#if 0
	float seek_x1 = x1 - minab;
	float seek_x2 = x2 + minab;
	float seek_y1 = y1 - minab;
	float seek_y2 = y2 + minab;
#endif
	float seek_x1 = x1;
	float seek_x2 = x2;
	float seek_y1 = y1;
	float seek_y2 = y2;

	llQuadList *seek = new llQuadList();

	if (!points) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: map [%s] has no pointlist", command_name, mapname);
		repeat_worker = false;
		return 0;
	}

	llQuadList *quads = points->GetQuadList();

	if (!quads) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: map [%s] has no quadlist", command_name, mapname);
		repeat_worker = false;
		return 0;
	}

	quads->GetQuads(&seek, seek_x1, seek_y1, seek_x2, seek_y2);

	if (x1 < map->GetX1() && x2 > map->GetX2()) {
		fraction *= (map->GetX2() - map->GetX1()) / (x2 - x1);
		x1 = map->GetX1();
		x2 = map->GetX2();
	} else if (x1 < map->GetX1()) {
		fraction *= (x2 - map->GetX1()) / (x2 - x1);
		x1 = map->GetX1();
	} else if (x2 > map->GetX2()) {
		fraction *= (map->GetX2() - x1) / (x2 - x1);
		x2 = map->GetX2();
	} 

	if (y1 < map->GetY1() && y2 > map->GetY2()) {
		fraction *= (map->GetY2() - map->GetY1()) / (y2 - y1);
		y1 = map->GetY1();
		y2 = map->GetY2();
	} else if (y1 < map->GetY1()) {
		fraction *= (y2 - map->GetY1()) / (y2 - y1);
		y1 = map->GetY1();
	} else if (y2 > map->GetY2()) {
		fraction *= (map->GetY2() - y1) / (y2 - y1);
		y2 = map->GetY2();
	} 

	nmax = int (float(nmax) * fraction);
	
	if (max) {
		//collect info about the existing points
		points_done = points->GetN(x1, y1, x2, y2);
		nmax -= points_done;
		_llLogger()->WriteNextLine(-LOG_INFO, "Selection has already %i vertices, set %i vertices (fraction=%f)", points_done, nmax, fraction);
	} else {
		_llLogger()->WriteNextLine(-LOG_INFO, "Set %i vertices (fraction=%f)", nmax, fraction);
	}

	
	map->InitRnd(map->GetRawX(x1), map->GetRawY(y1),
		map->GetRawX(x2), map->GetRawY(y2));

	for (int num_point=0; num_point<nmax; num_point++) {	    
		int maxtry       = 0, 
			maxtry_total = 0;

		if ((num_point % 1000) == 0 && num_point) 
			_llLogger()->WriteNextLine(-LOG_INFO, "[%i]", num_point);

loop:	    
		float x = map->GetCoordRndX();
		float y = map->GetCoordRndY();
		float z = map->GetZ(x,y);

		double ceiling;
		double value;

		double mean=0, num=0, num_real=0, empty=0;

		alg->GetValue(x, y, &value, &ceiling);

		if (empty > 1000) {
			_llLogger()->WriteNextLine(LOG_WARNING, "This selection seems to be empty, skipped after %i vertices", num_point);
			goto end;
		} else if (value < 0.0000001) { //filter very small
			empty++;
			goto loop; 
		}

		empty = 0;
		mean+=value;
		num++;

		float idealdist = minab;
		if (cellsize_m)
			idealdist = cellsize_m - (((cellsize_m-minab)/float(mean/num)) * float(value));

		if (idealdist < minab) idealdist = minab;

		if (ceiling > (10.f*mean/num)) 
			ceiling = (10.f*mean/num);  //fixed cutoff -> BUGBUG
		double test = double(rand())/double(RAND_MAX) * ceiling;
		if (test > value) { 
			goto loop; 
		}
		float mingrid_x = minab + 1.0f;
		float mingrid_y = minab + 1.0f;
		if (cellsize_x)
			mingrid_x = points->GetMinDistanceGrid(x, y, cellsize_x, 1);
		if (cellsize_y)
			mingrid_y = points->GetMinDistanceGrid(x, y, cellsize_y, 2);
		float mingrid = mingrid_x > mingrid_y ? mingrid_y : mingrid_x;
		float maxradius = cellsize_m;
		if (maxradius <= (minab+1.0f)) maxradius = minab + 1.0f;

		if (mingrid_x > minab && mingrid_y > minab) {
			float mindist = points->GetMinDistance(x, y, maxradius, seek); //time consuming!!!
			//std::cout << mindist << std::endl;

			if (mindist > minab || mindist < 0) {
				//if (mindist >= 0) {

				test = 2 * float(rand())/float(RAND_MAX) * idealdist;
				if (test> (mindist + mingrid))
					goto loop; 

				maxtry = 0;
				//all conditions fulfilled
				points->AddPoint(x, y, z);
				num_real++;
			} else { //1 -> see below
				if (maxtry<(nmax / 10) && maxtry_total<100*nmax) {
					maxtry++;
					maxtry_total++;
					goto loop;
				} else {
					_llLogger()->WriteNextLine(-LOG_WARNING, "Mesh is too dense:selection aborted after %i vertices", num_point);
					delete seek;
					return 1;
				}
			}
		} else { //2 -> this was done 2x on purpose to save time
			if (maxtry<(nmax / 10) && maxtry_total<100*nmax) {
				maxtry++;
				maxtry_total++;
				goto loop;
			} else {
				_llLogger()->WriteNextLine(-LOG_WARNING, "Mesh is too dense:selection aborted after %i vertices", num_point);
				delete seek;
				return 1;
			}
		}
	}
end:

#if 0
	if (!map->IsInMap(x, y)) {
		_llLogger()->WriteNextLine(LOG_ERROR,"Point (%f, %f) not in map", x, y);
	} else {
		points->AddPoint(x, y, map->GetZ(x, y));	
	}
#endif

	delete seek;
	return 1;
}