#include "..\include\llsplitat.h"
#include <string.h>
#include <stdio.h>

//constructor
llSplitAt::llSplitAt() : llTriMod() {

	SetCommandName("SplitAt");
}

int llSplitAt::RegisterOptions(void) {
	if (!llTriMod::RegisterOptions()) return 0;

	RegisterValue("-x", &x);
	RegisterValue("-y", &y);

	return 1;
}


int llSplitAt::Init(void) {
	if (!llTriMod::Init()) return 0;

	if (!Used("-x") && !Used("-y")) {
		_llLogger()->WriteNextLine(-LOG_ERROR,"%s: either -x or -y has to be defined", command_name);
		return 0;
	}

	if (Used("-x")) {
		triangles->DivideAt(true, x, heightmap);    	    
	}
	if (Used("-y")) {
		triangles->DivideAt(false, y, heightmap);    	    
	}

	_llLogger()->WriteNextLine(LOG_COMMAND,"%s: done", command_name);

	return 1;
}