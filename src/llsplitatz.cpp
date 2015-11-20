#include "../include/llsplitatz.h"

llSplitAtZ::llSplitAtZ() : llTriMod() {
	SetCommandName("SplitAtZ");
}

int llSplitAtZ::Prepare(void) {
	if (!llTriMod::Prepare()) return 0;

	below = above = block = 0;
	z = 0;

	return 1;
}

int llSplitAtZ::RegisterOptions(void) {
	if (!llTriMod::RegisterOptions()) return 0;

	RegisterValue("-z", &z, LLWORKER_OBL_OPTION);
	
	RegisterFlag("-removebelow", &below);
	RegisterFlag("-removeabove", &above);
	RegisterFlag("-block", &block);
	
	return 1;
}


int llSplitAtZ::Exec(void) {
	
	if (!llTriMod::Exec()) return 0;
	
	int num = 0;
	int flag = 0;
	if (below) flag = 1;
	if (above) flag = 2;	

	if (block) {
		num = triangles->DivideAtZBlock(_llUtils()->x00, _llUtils()->y00, _llUtils()->x11, _llUtils()->y11, z, flag);  
		_llLogger()->WriteNextLine(-LOG_INFO, "%i triangles modified/created/deleted", num);
	} else {
		num = triangles->DivideAtZ(_llUtils()->x00, _llUtils()->y00, _llUtils()->x11, _llUtils()->y11, z); 
		_llLogger()->WriteNextLine(-LOG_INFO, "%i triangles modified/created", num);
		num = triangles->RemoveZ(_llUtils()->x00, _llUtils()->y00, _llUtils()->x11, _llUtils()->y11, z, flag);  
		if (flag) _llLogger()->WriteNextLine(-LOG_INFO, "%i triangles removed", num);
	}

	return 1;
}