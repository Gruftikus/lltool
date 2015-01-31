#include "../include/llecho.h"

llEcho::llEcho() : llWorker() {
	SetCommandName("Echo");
}

int llEcho::Prepare(void) {
	if (!llWorker::Prepare()) return 0;

	text = NULL;

	return 1;
}

int llEcho::RegisterOptions(void) {
	if (!llWorker::RegisterOptions()) return 0;

	RegisterValue("-text", &text, LLWORKER_OBL_OPTION);

	return 1;
}

int llEcho::Exec(void) {
	if (!llWorker::Exec()) return 0;

	std::cout << text << std::endl;

	return 1;
}
