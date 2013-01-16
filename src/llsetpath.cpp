
#include <direct.h>

#include "..\include\llsetpath.h"

llSetPath::llSetPath() : llWorker() {
	SetCommandName("SetPath");
}

int llSetPath::RegisterOptions(void) {
	if (!llWorker::RegisterOptions()) return 0;

	RegisterValue("-path", &path, LLWORKER_OBL_OPTION);

	return 1;
}

int llSetPath::Exec(void) {
	llWorker::Exec();

	_llLogger()->WriteNextLine(LOG_COMMAND,"%s: change directory path to %s", command_name, path);
	_chdir(path);

	return 1;
}
