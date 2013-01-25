#include "../include/llfilter.h"
#include "../include/llmaplist.h"
#include "../include/llmakederivatives.h"


llFilter::llFilter() : llMapWorker() {
	SetCommandName("Filter");
}

int llFilter::Prepare(void) {
	if (!llMapWorker::Prepare()) return 0;

	targetname = NULL;
	makeshort = 0;
	overwrite = 0;
	makederivatives = 0;

	return 1;
}

int llFilter::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;

	RegisterValue("-target",          &targetname);
	RegisterValue("-n",               &dist);
	RegisterFlag ("-use16bit",        &makeshort);
	RegisterFlag ("-overwrite",       &overwrite);
	RegisterFlag ("-MakeDerivatives", &makederivatives);

	return 1;
}

int llFilter::Exec(void) {
	if (!llMapWorker::Exec()) return 0;

	if (!Used("-target")) {
		targetname = new char[strlen(mapname)+10];
		sprintf_s(targetname, strlen(mapname)+10, "%s_filtered", mapname);
	}

	llMap *newmap = _llMapList()->GetMap(targetname);

	if (newmap) {
		_llLogger()->WriteNextLine(-LOG_WARNING,"%s: map %s existing, going to delete it", command_name, targetname);
		_llMapList()->DeleteMap(targetname);
	}

	int widthx = map->GetWidthX();
	int widthy = map->GetWidthY();

	float defaultheight = map->GetDefaultHeight();
	newmap = new llMap(widthx, widthy, makeshort, defaultheight);
//	float minheight = defaultheight + 1.0f;

	for (int y=0; y<widthy; y++) {
		for (int x=0; x<widthx; x++) {
			int x1 = x - dist;
			int x2 = x + dist;
			int y1 = y - dist;
			int y2 = y + dist;
			if (x1 < 0) x1 = 0;
			if (y1 < 0) y1 = 0;
			if (x2 > (int(widthx)-1)) x2 = widthx-1;
			if (y2 > (int(widthy)-1)) y2 = widthy-1;
			float mean=0., num=0.;

			for (int  xx=x1; xx<=x2; xx++) {
				for (int  yy=y1; yy<=y2; yy++) {
					float height = map->GetElementRaw(xx,yy);
//					if (height > minheight) {
						mean +=  height;
						num++;
//					}
				}
			}
			if (num)
				newmap->SetElementRaw(x,y,mean/num);
			else
				//newmap->SetElementRaw(x,y,defaultheight);
				newmap->SetElementRaw(x,y,-15);
		}
	}

	newmap->SetCoordSystem(map->GetX1(), map->GetY1(), map->GetX2(), map->GetY2(), map->GetZScale());
	
	if (overwrite) {
		for (int y=0; y<widthy; y++) {
			for (int x=0; x<widthx; x++) {
				map->SetElementRaw(x, y, newmap->GetElementRaw(x,y));
			}
		}
		delete newmap;
		newmap = map;
	}
	
	//Filtered map shares the points, etc with its master
	llPointList    * points    = _llMapList()->GetPointList(mapname);
	llTriangleList * triangles = _llMapList()->GetTriangleList(mapname);
	llPolygonList  * polygons  = _llMapList()->GetPolygonList(mapname);

	_llMapList()->AddMap(targetname, newmap, points, triangles, polygons);

	if (makederivatives) {
		llMakeDerivatives *der = new llMakeDerivatives();
		if (!der->RegisterOptions()) return 0;
		
		if (!der->Exec()) {
			delete der;
			return 0;
		}
		_llMapList()->ExchangeMap(targetname, map);
		delete der;
	}

	return 1;
}
