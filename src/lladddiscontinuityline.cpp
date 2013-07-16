#include "../include/lladddiscontinuityline.h"

llAddDiscontinuityLine::llAddDiscontinuityLine() : llSet() {
	SetCommandName("AddDiscontinuityLine");
}

int llAddDiscontinuityLine::RegisterOptions(void) {
	if (!llSet::RegisterOptions()) return 0;

	RegisterValue("-x1", &x1, LLWORKER_OBL_OPTION);
	RegisterValue("-y1", &y1, LLWORKER_OBL_OPTION);
	RegisterValue("-x2", &x2, LLWORKER_OBL_OPTION);
	RegisterValue("-y2", &y2, LLWORKER_OBL_OPTION);

	return 1;
}


int llAddDiscontinuityLine::Exec(void) {
	if (!llSet::Exec()) return 0;

	llLineList *lines = _llMapList()->GetLineList(mapname);
	if (!lines) {
		_llLogger()->WriteNextLine(-LOG_FATAL, "%s: no line list in map [%s]", command_name, mapname);
		return 0;
	}

	return lines->AddLine(x1, y1, x2, y2);    

}