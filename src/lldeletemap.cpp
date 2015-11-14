#include "../include/lldeletemap.h"
#include "../include/llmaplist.h"
#include "../include/llmakederivatives.h"


llDeleteMap::llDeleteMap() : llMapWorker() {
	SetCommandName("DeleteMap");
}

int llDeleteMap::Prepare(void) {
	if (!llMapWorker::Prepare()) return 0;

	return 1;
}

int llDeleteMap::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;

	return 1;
}

int llDeleteMap::Exec(void) {
	if (!llMapWorker::Exec()) return 0;

	return _llMapList()->DeleteMap(mapname);

	return 1;
}
