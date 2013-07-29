#include "../include/llcreatemap.h"
#include "../include/llmaplist.h"

llCreateMap::llCreateMap() : llWorker() {
	SetCommandName("CreateMap");
}

int llCreateMap::Prepare(void) {
	if (!llWorker::Prepare()) return 0;

	mapname = NULL;
	z       = 1.0;
	even    = 0;

	return 1;
}

int llCreateMap::RegisterOptions(void) {
	if (!llWorker::RegisterOptions()) return 0;

	RegisterValue("-widthx",  &widthx, LLWORKER_OBL_OPTION);
	RegisterValue("-widthy",  &widthy, LLWORKER_OBL_OPTION);
	RegisterValue("-x1",      &x1);
	RegisterValue("-y1",      &y1);
	RegisterValue("-x2",      &x2);
	RegisterValue("-y2",      &y2);
	RegisterValue("-zscale",  &z);
	RegisterValue("-name",    &mapname);

	RegisterFlag ("-even",    &even);
	RegisterFlag ("-rgb",     &rgb);

	return 1;
}

int llCreateMap::Exec(void) {
	if (!llWorker::Exec()) return 0;

	if (!Used("-name")) mapname = "_heightmap";

	llMap *newmap = _llMapList()->GetMap(mapname);
	if (newmap) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: map %s existing", command_name, mapname);
		return 0;
	}

	if (!Used("-x1")) x1 = _llUtils()->x00;
	if (!Used("-x2")) x2 = _llUtils()->x11;
	if (!Used("-y1")) y1 = _llUtils()->y00;
	if (!Used("-y2")) y2 = _llUtils()->y11;

	llMap * heightmap = NULL;
	if (Used("-rgb"))
		heightmap = new llMap(widthx, widthy, MAP_COLOR);
	else
		heightmap = new llMap(widthx, widthy);
	if (even) heightmap->SetEven();
	heightmap->SetCoordSystem(x1, y1, x2, y2, z);

	_llUtils()->x00 = x1;
	_llUtils()->y00 = y1;
	_llUtils()->x11 = x2;
	_llUtils()->y11 = y2;

	llQuadList     *quads      = heightmap->GenerateQuadList();
	llPointList    *points     = new llPointList(0, quads); 
	llPolygonList  *polygons   = new llPolygonList(points, heightmap);
	llLineList     *lines      = new llLineList(0, points, heightmap);
	llTriangleList *triangles  = new llTriangleList(0, points);
		
	if (!Used("-name"))
		_llMapList()->AddMap("_heightmap", heightmap, points, triangles, polygons, lines);
	else
		_llMapList()->AddMap(mapname, heightmap, points, triangles, polygons, lines);

	return 1;
}
