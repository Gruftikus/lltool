#include "..\include\lladdvertextopolygon.h"


llAddVertexToPolygon::llAddVertexToPolygon() : llSet() {
	SetCommandName("AddVertexToPolygon");
}

int llAddVertexToPolygon::Prepare(void) {
	if (!llSet::Prepare()) return 0;
	return 1;
}

int llAddVertexToPolygon::RegisterOptions(void) {
	if (!llSet::RegisterOptions()) return 0;

	RegisterValue("-x1",   &x1,           LLWORKER_OBL_OPTION);
	RegisterValue("-y1",   &y1,           LLWORKER_OBL_OPTION);
	RegisterValue("-name", &polygon_name, LLWORKER_OBL_OPTION);

	return 1;
}


int llAddVertexToPolygon::Init(void) {
	if (!llSet::Init()) return 0;

	llPolygonList *polygons = _llMapList()->GetPolygonList(mapname);
	if (!polygons) {
		_llLogger()->WriteNextLine(-LOG_FATAL, "%s: no polygon list in map [%s]", command_name, mapname);
		return 0;
	}

	return polygons->AddVertexToPolygon(x1, y1, polygon_name);  
}