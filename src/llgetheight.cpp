#include "../include/llgetheight.h"
#include "../include/llmaplist.h"

llGetHeight::llGetHeight() : llMapWorker() {
	SetCommandName("GetHeight");
}

int llGetHeight::Prepare(void) {
	if (!llMapWorker::Prepare()) return 0;

	return 1;
}

int llGetHeight::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;

	RegisterValue("-x", &x, LLWORKER_OBL_OPTION);
	RegisterValue("-y", &y, LLWORKER_OBL_OPTION);

	return 1;
}

int llGetHeight::Exec(void) {
	if (!llMapWorker::Exec()) return 0;

	return _llUtils()->SetValue("_z", double(map->GetZ(x, y)));
}
