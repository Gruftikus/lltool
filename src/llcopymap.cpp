#include "../include/llcopymap.h"
#include "../include/llmaplist.h"
#include "../include/llmakederivatives.h"


llCopyMap::llCopyMap() : llMapWorker() {
	SetCommandName("CopyMap");
}

int llCopyMap::Prepare(void) {
	if (!llMapWorker::Prepare()) return 0;

	targetname = NULL;

	return 1;
}

int llCopyMap::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;

	RegisterValue("-name", &targetname);

	return 1;
}

int llCopyMap::Exec(void) {
	if (!llMapWorker::Exec()) return 0;

	if (!Used("-name")) {
		targetname = new char[strlen(mapname)+10];
		sprintf_s(targetname, strlen(mapname)+10, "%s_copy", mapname);
	}

	llMap *newmap = _llMapList()->GetMap(targetname);

	if (newmap) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: map %s existing", command_name, targetname);
		return 0;
	}

	//int widthx = map->GetWidthX();
	//int widthy = map->GetWidthY();

	int x1 = map->GetRawX(_llUtils()->x00);
	int y1 = map->GetRawY(_llUtils()->y00);
	int x2 = map->GetRawX(_llUtils()->x11);
	int y2 = map->GetRawY(_llUtils()->y11);

	float defaultheight = map->GetDefaultHeight();
	int   makeshort     = map->GetShort();
	newmap = new llMap(x2-x1+1, y2-y1+1, makeshort, defaultheight);
	newmap->SetCoordSystem(map->GetX1(), map->GetY1(), map->GetX2(), map->GetY2(), map->GetZScale());

	for (int y=y1; y<=y2; y++) {
		for (int x=x1; x<=x2; x++) {
			newmap->SetElementRaw(x-x1, y-y1, map->GetElementRaw(x,y));
		}
	}

	llPointList    * points    = _llMapList()->GetPointList(mapname);
	llTriangleList * triangles = _llMapList()->GetTriangleList(mapname);
	llPolygonList  * polygons  = _llMapList()->GetPolygonList(mapname);

	_llMapList()->AddMap(targetname, newmap, points, triangles, polygons);

	return 1;
}
