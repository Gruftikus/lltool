#include "..\include\llcreatemap.h"
#include "..\include\llmaplist.h"

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
	RegisterValue("-x1",      &x1,     LLWORKER_OBL_OPTION);
	RegisterValue("-y1",      &y1,     LLWORKER_OBL_OPTION);
	RegisterValue("-x2",      &x2,     LLWORKER_OBL_OPTION);
	RegisterValue("-y2",      &y2,     LLWORKER_OBL_OPTION);
	RegisterValue("-zscale",  &z);
	RegisterValue("-name",    &mapname);

	RegisterFlag ("-even",    &even);

	return 1;
}

int llCreateMap::Exec(void) {
	if (!llWorker::Exec()) return 0;

	llMap * heightmap = new llMap(widthx, widthy);
	if (even) heightmap->SetEven();
	heightmap->SetCoordSystem(x1, y1, x2, y2, z);

	_llUtils()->x00 = x1;
	_llUtils()->y00 = y1;
	_llUtils()->x11 = x2;
	_llUtils()->y11 = y2;

	llQuadList     *quads      = heightmap->GenerateQuadList();
	llPointList    *points     = new llPointList(0, quads); 
	llPolygonList  *polygons   = new llPolygonList(points, heightmap);
	llTriangleList *triangles  = new llTriangleList(0, points);
		
	if (!Used("-name"))
		_llMapList()->AddMap("_heightmap", heightmap, points, triangles, polygons);
	else
		_llMapList()->AddMap(mapname, heightmap, points, triangles, polygons);

	return 1;
}
