#include "../include/llsplitatz.h"

llSplitAtZ::llSplitAtZ() : llTriMod() {
	SetCommandName("SplitAtZ");
}

int llSplitAtZ::RegisterOptions(void) {
	if (!llTriMod::RegisterOptions()) return 0;

	RegisterValue("-z", &z, LLWORKER_OBL_OPTION);

	return 1;
}


int llSplitAtZ::Exec(void) {
	if (!llTriMod::Exec()) return 0;

	int num = triangles->DivideAtZ(_llUtils()->x00, _llUtils()->y00, _llUtils()->x11, _llUtils()->y11, z);  
	_llLogger()->WriteNextLine(-LOG_INFO, "%i triangles modified/created", num);

	return 1;
}