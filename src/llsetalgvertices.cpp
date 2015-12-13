#include "../include/llsetalgvertices.h"
#include "../include/llalglist.h"


llSetAlgVertices::llSetAlgVertices() : llSet() {
	SetCommandName("SetAlgVertices");
}

int llSetAlgVertices::Prepare(void) {
	if (!llSet::Prepare()) return 0;

	max      = 0;
	cutoff   = 10;
	alg_list = (char *)"_heightmap_alg";

	return 1;
}

int llSetAlgVertices::RegisterOptions(void) {
	if (!llSet::RegisterOptions()) return 0;

	RegisterValue("-n",      &nmax, LLWORKER_OBL_OPTION);
	RegisterValue("-alg",    &alg_list);
	RegisterValue("-cutoff", &cutoff);
	RegisterFlag ("-max",    &max);

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

	float threshold = 0;
	if (_llUtils()->GetValueF("_density_threshold"))
		threshold = (float)(*_llUtils()->GetValueF("_density_threshold"));
	threshold = 1.0 - (threshold / 100.0);

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

	double mean=0, num=0, num_real=0, empty=0;
	double ceiling = 0;

	float mingrid_x = minab_grid + 1.0f;
	float mingrid_y = minab_grid + 1.0f;
	float mingrid = mingrid_x > mingrid_y ? mingrid_y : mingrid_x;
	float maxradius = cellsize_m;
	if (maxradius <= (minab+1.0f)) maxradius = minab + 1.0f;

	_llLogger()->WriteNextLine(-LOG_INFO, "Generating cache....");
	int widthx = map->GetRawX(x2) - map->GetRawX(x1) + 1;
	int widthy = map->GetRawY(y2) - map->GetRawY(y1) + 1;
	llMap *tmpmap = new llMap(widthx, widthy);
	tmpmap->SetCoordSystem(x1, y1, x2, y2, 1);
	for (unsigned int x=0; x<widthx; x++) {
		for (unsigned int y=0; y<widthy; y++) {
			double value   = 1;
			alg->GetValue(tmpmap->GetCoordX(x), tmpmap->GetCoordY(y), &value);
			value *= value;
			tmpmap->SetElementRaw(x, y, value);
			if (value) {
				num++;
				mean += value;
				if (value > ceiling) ceiling = value;
			}

			if (cellsize_x)
				mingrid_x = points->GetMinDistanceGrid(tmpmap->GetCoordX(x), tmpmap->GetCoordY(y), cellsize_x, 1);
			if (cellsize_y)
				mingrid_y = points->GetMinDistanceGrid(tmpmap->GetCoordX(x), tmpmap->GetCoordY(y), cellsize_y, 2);

			if (mingrid_x <= minab_grid || mingrid_y <= minab_grid) {
				tmpmap->SetElementRaw(x, y, 0);
			}
		}
	}

	int n = points->GetN();

	for (int i=0; i<n; i++) {
		if (points->GetX(i) >= x1 && points->GetX(i) <= x2 && points->GetY(i) >= y1 && points->GetY(i) <= y2) {
			tmpmap->CutCircle(points->GetX(i), points->GetY(i), minab);
		}
	}

	double integral = tmpmap->MakeIntegral();
	double running_integral = integral;

	_llLogger()->WriteNextLine(-LOG_INFO, "....done");

	if (!mean || !num || !integral) {
		_llLogger()->WriteNextLine(LOG_WARNING, "This selection seems to be empty, skipped");
		goto end;
	} 

	mean /= num;
	if (ceiling > (cutoff*mean)) 
		ceiling = (cutoff*mean);  

	//std::cout << integral << ":" << ceiling << std::endl;

	for (int num_point=0; num_point<nmax; num_point++) {	    
		int maxtry       = 0, 
			maxtry_total = 0;

		if ((num_point % 1000) == 0 && num_point) 
			_llLogger()->WriteNextLine(-LOG_INFO, "[%i]", num_point);

loop:	    
		unsigned int xpos = tmpmap->GetRndX();
		unsigned int ypos = tmpmap->GetRndY();
		double value = tmpmap->GetZ(xpos, ypos);

		//std::cout << xpos << ":" << ypos << ":" <<  value << std::endl;

		if ((running_integral / integral) < threshold) {
			_llLogger()->WriteNextLine(LOG_WARNING, "Density reached %.2f percent, skipped", (1.0 - running_integral/integral) * 100);
			goto end;
		} 

		if (empty > 1000) {
			_llLogger()->WriteNextLine(LOG_WARNING, "Endless loop? Skipped after %i vertices", num_point);
			goto end;
		} else if (value < 0.0000001) { //filter very small
			empty++;
			goto loop; 
		}

		empty = 0;

		double test;
#if 1
		test = double(rand())/double(RAND_MAX) * ceiling;
		if (test > value) { 
			//std::cout << "repeat, value=" << value << ", ceiling=" << ceiling << std::endl;
			goto loop; 
		}
#endif

#if 0
		float idealdist = minab;
		if (cellsize_m)
			idealdist = cellsize_m - (((cellsize_m-minab)/float(mean)) * float(value));
		if (idealdist < minab) idealdist = minab;
#endif

		float x = tmpmap->GetCoordX(xpos);
		float y = tmpmap->GetCoordY(ypos);
		float z = map->GetZ(x, y);

#if 0
		test = 2 * float(rand())/float(RAND_MAX) * idealdist;
		if (test> (mindist + mingrid))
			goto loop; 
#endif

		//all conditions fulfilled
		points->AddPoint(x, y, z);

		//hole-shooting algorithm
		running_integral -= tmpmap->CutCircle(x, y, minab);

		num_real++;

	}


end:

#if 0
		if (!map->IsInMap(x, y)) {
			_llLogger()->WriteNextLine(LOG_ERROR,"Point (%f, %f) not in map", x, y);
		} else {
			points->AddPoint(x, y, map->GetZ(x, y));	
		}
#endif

		delete tmpmap;
		//_llMapList()->AddMap("tmp" , tmpmap, NULL, NULL, NULL, NULL);
		delete seek;
		return 1;
	}