//base class for all workers which sets vertex points

#include "..\include\llset.h"
#include "..\include\llmaplist.h"


llSet::llSet() : llWorker() {
}

int llSet::Prepare(void) {
	if (!llWorker::Prepare()) return 0;

	mapname = NULL;

	return 1;
}

int llSet::RegisterOptions(void) {
	if (!llWorker::RegisterOptions()) return 0;

	RegisterValue("-map",  &mapname);

	return 1;
}

int llSet::Exec(void) {
	llWorker::Exec();

	if (!Used("-map"))
		mapname = "_heightmap";

	//get the corresponding map from the global map container
	map = _llMapList()->GetMap(mapname);
	if (!map) {
		_llLogger()->WriteNextLine(-LOG_FATAL, "%s: map [%s] not found", command_name, mapname);
		return 0;
	}

	llTriangleList *triangles = _llMapList()->GetTriangleList(mapname);
	if (triangles && triangles->GetN()) {
		_llLogger()->WriteNextLine(-LOG_FATAL, "%s: map [%s] already triangulated", command_name, mapname);
		return 0;
	}

	points = _llMapList()->GetPointList(mapname);
	if (!points) {
		_llLogger()->WriteNextLine(-LOG_FATAL, "%s: no vertex points in map [%s]", command_name, mapname);
		return 0;
	}

	return 1;
}
