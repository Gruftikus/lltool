#include "..\include\llsplitatgrid.h"

llSplitAtGrid::llSplitAtGrid() : llTriMod() {
	SetCommandName("SplitAtGrid");
}

int llSplitAtGrid::Prepare(void) {
	if (!llTriMod::Prepare()) return 0;
	return 1;
}

int llSplitAtGrid::RegisterOptions(void) {
	if (!llTriMod::RegisterOptions()) return 0;

	RegisterValue("-x", &gridx);
	RegisterValue("-y", &gridy);

	return 1;
}


int llSplitAtGrid::Init(void) {
	if (!llTriMod::Init()) return 0;

	if (!Used("-x") && !Used("-y")) {
		_llLogger()->WriteNextLine(-LOG_ERROR,"%s: either -x or -y has to be defined", command_name);
		return 0;
	}

	if (Used("-x")) {
		for (float y=floor(_llUtils()->y00/gridy)*gridy; y<=_llUtils()->y11; y+=gridy) {
			triangles->DivideAt(false, y, map);    
		}
	}

	if (Used("-y")) {
		for (float x=floor(_llUtils()->x00/gridx)*gridx; x<=_llUtils()->x11; x+=gridx) {
			triangles->DivideAt(true, x, map);    
		}
	}

	_llLogger()->WriteNextLine(LOG_COMMAND,"%s: done", command_name);

	return 1;
}