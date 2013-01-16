//base class for all workers which sets vertex points

#include "..\include\lltrimod.h"
#include "..\include\llmaplist.h"


llTriMod::llTriMod() : llWorker() {	
}

int llTriMod::Prepare(void) {
	if (!llWorker::Prepare()) return 0;

	mapname   = NULL;
	triangles = NULL;
	points    = NULL;

	return 1;
}

int llTriMod::RegisterOptions(void) {
	if (!llWorker::RegisterOptions()) return 0;

	RegisterValue("-map",  &mapname);

	return 1;
}

int llTriMod::Exec(void) {
	llWorker::Exec();

	if (!Used("-map"))
		mapname = "_heightmap";

	//get the corresponding map from the global map container
	map = _llMapList()->GetMap(mapname);
	if (!map) {
		_llLogger()->WriteNextLine(-LOG_FATAL, "%s: map [%s] not found", command_name, mapname);
		return 0;
	}

	triangles = _llMapList()->GetTriangleList(mapname);
	if (!triangles) {
		_llLogger()->WriteNextLine(-LOG_FATAL, "%s: no triangle list in map [%s]", command_name, mapname);
		return 0;
	}
	if (!triangles->GetN()) {
		_llLogger()->WriteNextLine(-LOG_FATAL, "%s: map [%s] not triangulated. Call MakeTriangulation before.", command_name, mapname);
		return 0;
	}

	points = _llMapList()->GetPointList(mapname);
	if (!points) {
		_llLogger()->WriteNextLine(-LOG_FATAL, "%s: no vertex points in map [%s]", command_name, mapname);
		return 0;
	}

	return 1;
}
