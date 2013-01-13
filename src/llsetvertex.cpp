#include "..\include\llsetvertex.h"

llSetVertex::llSetVertex() : llSet() {
	SetCommandName("SetVertex");
}

int llSetVertex::RegisterOptions(void) {
	if (!llSet::RegisterOptions()) return 0;

	RegisterValue("-x", &x, LLWORKER_OBL_OPTION);
	RegisterValue("-y", &y, LLWORKER_OBL_OPTION);

	return 1;
}


int llSetVertex::Init(void) {
	if (!llSet::Init()) return 0;

	if (!map->IsInMap(x, y)) {
		_llLogger()->WriteNextLine(LOG_ERROR,"Point (%f, %f) not in map", x, y);
	} else {
		points->AddPoint(x, y, map->GetZ(x, y));	
	}

	return 1;
}