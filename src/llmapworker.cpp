//base class for all workers which sets vertex points

#include "../include/llmapworker.h"
#include "../include/llmaplist.h"


llMapWorker::llMapWorker() : llWorker() {
}

int llMapWorker::Prepare(void) {
	if (!llWorker::Prepare()) return 0;

	map     = NULL;
	mapname = NULL;

	return 1;
}

int llMapWorker::RegisterOptions(void) {
	if (!llWorker::RegisterOptions()) return 0;

	RegisterValue("-map",  &mapname);

	return 1;
}

int llMapWorker::Exec(void) {
	llWorker::Exec();

	if (!Used("-map"))
		mapname = (char *)"_heightmap";

	//get the corresponding map from the global map container
	map = _llMapList()->GetMap(mapname);
	if (!map) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: map [%s] not found", command_name, mapname);
		return 0;
	}

	triangles = _llMapList()->GetTriangleList(mapname);
	points    = _llMapList()->GetPointList(mapname);
	polygons  = _llMapList()->GetPolygonList(mapname);

	return 1;
}
