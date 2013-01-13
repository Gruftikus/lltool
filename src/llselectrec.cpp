#include "..\include\llselectrec.h"

llSelectRec::llSelectRec() : llWorker() {
	SetCommandName("SelectRec");
}

int llSelectRec::RegisterOptions(void) {
	if (!llWorker::RegisterOptions()) return 0;

	RegisterValue("-x1", &x1, LLWORKER_OBL_OPTION);
	RegisterValue("-x2", &x2, LLWORKER_OBL_OPTION);
	RegisterValue("-y1", &y1, LLWORKER_OBL_OPTION);
	RegisterValue("-y2", &y2, LLWORKER_OBL_OPTION);

	return 1;
}

int llSelectRec::Init(void) {
	llWorker::Init();

	_llUtils()->x00 = x1;
	_llUtils()->x11 = x2;
	_llUtils()->y00 = y1;
	_llUtils()->y11 = y2;

	return 1;
}
