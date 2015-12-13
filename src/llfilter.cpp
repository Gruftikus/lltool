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
	rgb = 0;

	return 1;
}

int llFilter::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;

	RegisterValue("-name",            &targetname);
	RegisterValue("-n",               &dist, LLWORKER_OBL_OPTION);
	RegisterFlag ("-use16bit",        &makeshort);
	RegisterFlag ("-rgb",             &rgb);
	RegisterFlag ("-overwrite",       &overwrite);
	RegisterFlag ("-MakeDerivatives", &makederivatives);

	return 1;
}

int llFilter::Exec(void) {
	if (!llMapWorker::Exec()) return 0;

	if (!Used("-name")) {
		targetname = new char[strlen(mapname)+10];
		sprintf_s(targetname, strlen(mapname)+10, "%s_filtered", mapname);
	}

	llMap *newmap = _llMapList()->GetMap(targetname);

	if (newmap) {
		_llLogger()->WriteNextLine(-LOG_WARNING, "%s: map %s existing, going to delete it", command_name, targetname);
		_llMapList()->DeleteMap(targetname);
	}

	unsigned int widthx = map->GetWidthX();
	unsigned int widthy = map->GetWidthY();

	float threshold = 100000/map->GetZScale();

	float defaultheight = map->GetDefaultHeight();
	if (rgb)
		newmap = new llMap(widthx, widthy, MAP_COLOR);
	else
		newmap = new llMap(widthx, widthy, makeshort, defaultheight);
	if (map->IsEven()) newmap->SetEven();

//	float minheight = defaultheight + 1.0f;

	for (unsigned int y=0; y<widthy; y++) {
		for (unsigned int x=0; x<widthx; x++) {
			float orig_height = 0;
			if (!rgb) orig_height = map->GetElementRaw(x, y);
			int x1 = x - dist;
			int x2 = x + dist;
			int y1 = y - dist;
			int y2 = y + dist;
			if (x1 < 0) x1 = 0;
			if (y1 < 0) y1 = 0;
			if (x2 > (int(widthx)-1)) x2 = widthx-1;
			if (y2 > (int(widthy)-1)) y2 = widthy-1;
			float mean=0., num=0.;
			float mean_red=0., mean_blue=0., mean_green=0., mean_alpha=0.;
			unsigned char red, blue, green, alpha;

			if (rgb || (int(orig_height) != int(defaultheight))) {
				for (int  xx=x1; xx<=x2; xx++) {
					for (int  yy=y1; yy<=y2; yy++) {
						if (rgb) {
							map->GetTupel(xx, yy, &blue, &green, &red, &alpha);
							mean_blue  += (float)blue;
							mean_green += (float)green;
							mean_red   += (float)red;
							mean_alpha += (float)alpha;
							num++;
						} else {
							float height = map->GetElementRaw((unsigned int) xx, (unsigned int) yy);
							if (fabs(height - orig_height) < threshold && (int(height) != int(defaultheight))) {
								mean +=  height;
								num++;
							}
						}
					}
				}
			}
			if (rgb) {
				blue  = (unsigned char)(mean_blue/num);
				green = (unsigned char)(mean_green/num);
				red   = (unsigned char)(mean_red/num);
				alpha = (unsigned char)(mean_alpha/num);
				newmap->SetTupel(x, y, blue, green, red, alpha);
			} else if (num) {
				newmap->SetElementRaw(x, y, mean/num);
				//std::cout << (int)(mean/num) << ":" << (int)orig_height << "  ";
			} else {
				//newmap->SetElementRaw(x,y,defaultheight);
				newmap->SetElementRaw(x, y, orig_height);
			}
		}
	}

	newmap->SetCoordSystem(map->GetX1(), map->GetY1(), map->GetX2(), map->GetY2(), map->GetZScale());
	
	if (overwrite) {
		for (unsigned int y=0; y<widthy; y++) {
			for (unsigned int x=0; x<widthx; x++) {
				map->SetElementRaw(x, y, newmap->GetElementRaw(x,y));
			}
		}
		delete newmap;
		newmap = map;
	}
	
	//Filtered map shares the points, etc with its master
	llPointList    * points    = _llMapList()->GetPointList(mapname);
	llLineList     * lines     = _llMapList()->GetLineList(mapname);
	llTriangleList * triangles = _llMapList()->GetTriangleList(mapname);
	llPolygonList  * polygons  = _llMapList()->GetPolygonList(mapname);

	_llMapList()->AddMap(targetname, newmap, points, triangles, polygons, lines);

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
