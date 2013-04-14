#include "../include/lldiamondsquare.h"
#include "../include/llmaplist.h"
#include "../include/llalglist.h"

//constructor
llDiamondSquare::llDiamondSquare() : llMapWorker() {
	SetCommandName("DiamondSquare");	
}

int llDiamondSquare::Prepare(void) {
	if (!llMapWorker::Prepare()) return 0;

	alg_list = NULL;
	offset   = 0.;

	return 1;
}

int llDiamondSquare::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;

	RegisterValue("-alg",     &alg_list);
	RegisterValue("-offset",  &offset);
	RegisterValue("-range",   &range, LLWORKER_OBL_OPTION);

	return 1;
}

int llDiamondSquare::Exec(void) {
	if (!llMapWorker::Exec()) return 0;

	llAlgCollection *algs = NULL;
	if (Used("-alg")) {
		algs = _llAlgList()->GetAlgCollection(alg_list);
		if (!algs) {
			_llLogger()->WriteNextLine(-LOG_FATAL, "%s: alg collection '%s' not found", command_name, alg_list);
			return 0;
		}
	}

//	float range_alg = range;
	double alg = 1.;
	offset = 0.5f - offset;

	unsigned int old_widthx = map->GetWidthX();
	unsigned int old_widthy = map->GetWidthY();

	unsigned int new_widthx = (old_widthx - 1) * 2 + 1;
	unsigned int new_widthy = (old_widthy - 1) * 2 + 1;

	llMap * heightmap = new llMap(new_widthx, new_widthy);
	heightmap->SetCoordSystem(map->GetX1(), map->GetY1(), map->GetX2(), map->GetY2(), map->GetZScale());

	//Expansion step
	for (unsigned int x=0; x<old_widthx; x++) {
		for (unsigned int y=0; y<old_widthy; y++) {
			heightmap->SetElementRaw(2*x, 2*y, map->GetElementRaw(x,y));
		}
	}

	//Mean step
	for (unsigned int x=0; x<old_widthx-1; x++) {
		for (unsigned int y=0; y<old_widthy-1; y++) {
			alg = 1.0;
			if (algs) alg = algs->GetValue(heightmap->GetCoordX(2*x+1), heightmap->GetCoordY(2*y+1));
			
			double alg00 = 1.0;
			if (algs) alg00 = 1. - fabs(algs->GetValue(map->GetCoordX(x), map->GetCoordY(y)) - alg);
			if (alg00 < 0.) alg00 = 0.;
			double alg10 = 1.0;
			if (algs) alg10 = 1. - fabs(algs->GetValue(map->GetCoordX(x+1), map->GetCoordY(y)) - alg);
			if (alg10 < 0.) alg10 = 0.;
			double alg01 = 1.0;
			if (algs) alg01 = 1. - fabs(algs->GetValue(map->GetCoordX(x), map->GetCoordY(y+1)) - alg);
			if (alg01 < 0.) alg01 = 0.;
			double alg11 = 1.0;
			if (algs) alg11 = 1. - fabs(algs->GetValue(map->GetCoordX(x+1), map->GetCoordY(y+1)) - alg);
			if (alg11 < 0.) alg11 = 0.;

			double mean;
			if (alg00+alg10+alg01+alg11)			
				mean = (alg00*map->GetElementRaw(x,y)
				+ alg10*map->GetElementRaw(x+1,y)
				+ alg01*map->GetElementRaw(x,y+1)
				+ alg11*map->GetElementRaw(x+1,y+1)) / (alg00+alg10+alg01+alg11);
			else
				mean = (map->GetElementRaw(x,y)
				+ map->GetElementRaw(x+1,y)
				+ map->GetElementRaw(x,y+1)
				+ map->GetElementRaw(x+1,y+1)) / 4.0;

//t:
			mean += range*alg*(float(rand())/float(RAND_MAX) - offset);
#if 0
			if (map->GetElementRaw(x,y) > mean &&
				map->GetElementRaw(x+1,y) > mean &&
				map->GetElementRaw(x,y+1) > mean &&
				map->GetElementRaw(x+1,y+1) > mean) {
					offset -= 0.1f;
					goto t;
			}
#endif
			heightmap->SetElementRaw(2*x+1, 2*y+1, mean);
		}
	}
	
	offset = 0.5;

	//Diamond step
	for (unsigned int x=0; x<old_widthx; x++) {
		for (unsigned int y=0; y<old_widthy-1; y++) {
			if (x==0) {
				alg = 1.0;
				if (algs) alg = algs->GetValue(heightmap->GetCoordX(0), heightmap->GetCoordY(2*y+1));

				double alg00 = 1.0;
				if (algs) alg00 = 1. - fabs(algs->GetValue(heightmap->GetCoordX(0), heightmap->GetCoordY(2*y)) - alg);
				if (alg00 < 0.) alg00 = 0.;
				double alg02 = 1.0;
				if (algs) alg02 = 1. - fabs(algs->GetValue(heightmap->GetCoordX(0), heightmap->GetCoordY(2*y+2)) - alg);
				if (alg02 < 0.) alg02 = 0.;

				double mean;
				if (alg+alg00+alg02)			
					mean = (alg00*heightmap->GetElementRaw(0,2*y)
					+ alg02*heightmap->GetElementRaw(0,2*y+2)
					+ alg*heightmap->GetElementRaw(1,2*y+1)) / (alg+alg00+alg02);
				else
					mean = (heightmap->GetElementRaw(0,2*y)
					+ heightmap->GetElementRaw(0,2*y+2)
					+ alg*heightmap->GetElementRaw(1,2*y+1)) / 3.0;
				mean += range*alg*(float(rand())/float(RAND_MAX)-offset);
				heightmap->SetElementRaw(0, 2*y+1, mean);
			} else if (x==(old_widthx-1)) {
				alg = 1.0;
				if (algs) alg = algs->GetValue(heightmap->GetCoordX(2*x), heightmap->GetCoordY(2*y+1));

				double alg00 = 1.0;
				if (algs) alg00 = 1. - fabs(algs->GetValue(heightmap->GetCoordX(2*x), heightmap->GetCoordY(2*y)) - alg);
				if (alg00 < 0.) alg00 = 0.;
				double alg02 = 1.0;
				if (algs) alg02 = 1. - fabs(algs->GetValue(heightmap->GetCoordX(2*x), heightmap->GetCoordY(2*y+2)) - alg);
				if (alg02 < 0.) alg02 = 0.;

				double mean;
				if (alg+alg00+alg02)			
					mean = (alg00*heightmap->GetElementRaw(2*x,2*y)
					+ alg02*heightmap->GetElementRaw(2*x,2*y+2)
					+ alg*heightmap->GetElementRaw(2*x-1,2*y+1))/(alg+alg00+alg02);
				else
					mean = (heightmap->GetElementRaw(2*x,2*y)
					+ heightmap->GetElementRaw(2*x,2*y+2)
					+ heightmap->GetElementRaw(2*x-1,2*y+1))/(3.0);
				mean += range*alg*(float(rand())/float(RAND_MAX)-offset);
				heightmap->SetElementRaw(2*x, 2*y+1, mean);
			} else {
				alg = 1.0;
				if (algs) alg = algs->GetValue(heightmap->GetCoordX(2*x), heightmap->GetCoordY(2*y+1));

				double alg00 = 1.0;
				if (algs) alg00 = 1. - fabs(algs->GetValue(heightmap->GetCoordX(2*x), heightmap->GetCoordY(2*y)) - alg);
				if (alg00 < 0.) alg00 = 0.;
				double alg02 = 1.0;
				if (algs) alg02 = 1. - fabs(algs->GetValue(heightmap->GetCoordX(2*x), heightmap->GetCoordY(2*y+2)) - alg);
				if (alg02 < 0.) alg02 = 0.;

				double mean;
				if (alg+alg00+alg02)			
					mean = (alg00*heightmap->GetElementRaw(2*x,2*y)
					+ alg02*heightmap->GetElementRaw(2*x,2*y+2)
					+ alg*heightmap->GetElementRaw(2*x-1,2*y+1)
					+ alg*heightmap->GetElementRaw(2*x-1,2*y-1))/(alg00+alg02+alg+alg);
				else
					mean = (heightmap->GetElementRaw(2*x,2*y)
					+ heightmap->GetElementRaw(2*x,2*y+2)
					+ heightmap->GetElementRaw(2*x-1,2*y+1)
					+ heightmap->GetElementRaw(2*x-1,2*y-1))/(4.0);
//u:
				mean += range*alg*(float(rand())/float(RAND_MAX)-offset);
#if 0
				if (heightmap->GetElementRaw(2*x,2*y) > mean &&
					heightmap->GetElementRaw(2*x,2*y+2) > mean &&
					heightmap->GetElementRaw(2*x-1,2*y+1) > mean &&
					heightmap->GetElementRaw(2*x-1,2*y-1)) {
						offset -= 0.1f;
						goto u;
				}
#endif
				heightmap->SetElementRaw(2*x, 2*y+1, mean);
			} 
		}
	}

	for (unsigned int y=0; y<old_widthy; y++) {
		for (unsigned int x=0; x<old_widthx-1; x++) {
			if (y==0) {
				alg = 1.0;
				if (algs) alg = algs->GetValue(heightmap->GetCoordX(2*x+1), heightmap->GetCoordY(0));

				double alg00 = 1.0;
				if (algs) alg00 = 1. - fabs(algs->GetValue(heightmap->GetCoordX(2*x), heightmap->GetCoordY(2*y)) - alg);
				if (alg00 < 0.) alg00 = 0.;
				double alg20 = 1.0;
				if (algs) alg20 = 1. - fabs(algs->GetValue(heightmap->GetCoordX(2*x+2), heightmap->GetCoordY(2*y)) - alg);
				if (alg20 < 0.) alg20 = 0.;

				double mean;				
				if (alg+alg00+alg20)			
					mean = (alg00*heightmap->GetElementRaw(2*x,0)
					+ alg20*heightmap->GetElementRaw(2*x+2,0)
					+ alg*heightmap->GetElementRaw(2*x+1,1))/(alg+alg00+alg20);
				else
					mean = (heightmap->GetElementRaw(2*x,0)
					+ heightmap->GetElementRaw(2*x+2,0)
					+ heightmap->GetElementRaw(2*x+1,1))/(3.0);
				mean += range*alg*(float(rand())/float(RAND_MAX)-offset);
				heightmap->SetElementRaw(2*x+1, 0, mean);
			} else if (y==(old_widthy-1)) {
				alg = 1.0;
				if (algs) alg = algs->GetValue(heightmap->GetCoordX(2*x+1), heightmap->GetCoordY(2*y));

				double alg00 = 1.0;
				if (algs) alg00 = 1. - fabs(algs->GetValue(heightmap->GetCoordX(2*x), heightmap->GetCoordY(2*y)) - alg);
				if (alg00 < 0.) alg00 = 0.;
				double alg20 = 1.0;
				if (algs) alg20 = 1. - fabs(algs->GetValue(heightmap->GetCoordX(2*x+2), heightmap->GetCoordY(2*y)) - alg);
				if (alg20 < 0.) alg20 = 0.;

				double mean;
				if (alg+alg00+alg20)			
					mean = (alg00*heightmap->GetElementRaw(2*x,2*y)
					+ alg20*heightmap->GetElementRaw(2*x+2,2*y)
					+ alg*heightmap->GetElementRaw(2*x+1,2*y-1))/(alg+alg00+alg20);
				else
					mean = (heightmap->GetElementRaw(2*x,2*y)
					+ heightmap->GetElementRaw(2*x+2,2*y)
					+ heightmap->GetElementRaw(2*x+1,2*y-1))/(3.0);
				mean += range*alg*(float(rand())/float(RAND_MAX)-offset);
				heightmap->SetElementRaw(2*x+1, 2*y, mean);
			} else {
				alg = 1.0;
				if (algs) alg = algs->GetValue(heightmap->GetCoordX(2*x+1), heightmap->GetCoordY(2*y));

								double alg00 = 1.0;
				if (algs) alg00 = 1. - fabs(algs->GetValue(heightmap->GetCoordX(2*x), heightmap->GetCoordY(2*y)) - alg);
				if (alg00 < 0.) alg00 = 0.;
				double alg20 = 1.0;
				if (algs) alg20 = 1. - fabs(algs->GetValue(heightmap->GetCoordX(2*x+2), heightmap->GetCoordY(2*y)) - alg);
				if (alg20 < 0.) alg20 = 0.;

				double mean;
				if (alg+alg00+alg20)			
					mean = (alg00*heightmap->GetElementRaw(2*x,2*y)
					+ alg20*heightmap->GetElementRaw(2*x+2,2*y)
					+ alg*heightmap->GetElementRaw(2*x+1,2*y-1)
					+ alg*heightmap->GetElementRaw(2*x+1,2*y+1))/(alg00+alg20+alg+alg);
				else
					mean = (heightmap->GetElementRaw(2*x,2*y)
					+ heightmap->GetElementRaw(2*x+2,2*y)
					+ heightmap->GetElementRaw(2*x+1,2*y-1)
					+ heightmap->GetElementRaw(2*x+1,2*y+1))/(4.0);
//v:
				mean += range*alg*(float(rand())/float(RAND_MAX)-offset);
#if 0
				if (heightmap->GetElementRaw(2*x,2*y) > mean &&
					+ heightmap->GetElementRaw(2*x+2,2*y) > mean &&
					+ heightmap->GetElementRaw(2*x+1,2*y-1) > mean &&
					+ heightmap->GetElementRaw(2*x+1,2*y+1) > mean) {
						offset -= 0.1f;
						goto v;
				}
#endif
				heightmap->SetElementRaw(2*x+1, 2*y, mean);
			} 
		}
	}

	delete (map);
	_llMapList()->ExchangeMap(mapname, heightmap);

	return 1;
}
