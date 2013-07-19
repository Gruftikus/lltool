#include "../include/llsplitbetween.h"

llSplitBetween::llSplitBetween() : llTriMod() {
	SetCommandName("SplitBetween");
}

int llSplitBetween::RegisterOptions(void) {
	if (!llTriMod::RegisterOptions()) return 0;

	RegisterValue("-x1", &x1, LLWORKER_OBL_OPTION);
	RegisterValue("-y1", &y1, LLWORKER_OBL_OPTION);
	RegisterValue("-x2", &x2, LLWORKER_OBL_OPTION);
	RegisterValue("-y2", &y2, LLWORKER_OBL_OPTION);

	return 1;
}


int llSplitBetween::Exec(void) {
	if (!llTriMod::Exec()) return 0;

	triangles->DivideBetween(x1, y1, x2, y2, map);  

	//_llLogger()->WriteNextLine(LOG_COMMAND,"%s: done", command_name);

	return 1;
}