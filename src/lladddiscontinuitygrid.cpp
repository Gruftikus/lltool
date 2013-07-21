#include "../include/lladddiscontinuitygrid.h"

llAddDiscontinuityGrid::llAddDiscontinuityGrid() : llSet() {
	SetCommandName("AddDiscontinuityGrid");
}

int llAddDiscontinuityGrid::RegisterOptions(void) {
	if (!llSet::RegisterOptions()) return 0;

	RegisterValue("-x", &gridx, LLWORKER_OBL_OPTION);
	RegisterValue("-y", &gridy, LLWORKER_OBL_OPTION);
	
	return 1;
}


int llAddDiscontinuityGrid::Exec(void) {
	if (!llSet::Exec()) return 0;

	llLineList *lines = _llMapList()->GetLineList(mapname);
	if (!lines) {
		_llLogger()->WriteNextLine(-LOG_FATAL, "%s: no line list in map [%s]", command_name, mapname);
		return 0;
	}

	for (float x=floor(_llUtils()->x00/gridy)*gridy; x<=(_llUtils()->x11); x+=gridy) {
		float x1=x;
		if (x >= _llUtils()->x11) x1 = _llUtils()->x11;
		std::cout << x1 << std::endl;
		lines->AddLine(x1, _llUtils()->y00, x1, _llUtils()->y11);    
	}
	for (float y=floor(_llUtils()->y00/gridx)*gridx; y<=(_llUtils()->y11); y+=gridx) {
		float y1=y;
		if (y >= _llUtils()->y11) y1 = _llUtils()->y11;
		std::cout << y1 << std::endl;
		lines->AddLine(_llUtils()->x00, y1, _llUtils()->x11, y1); 	}

	return 1;

}