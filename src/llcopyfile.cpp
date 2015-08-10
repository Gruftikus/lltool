#include <stdlib.h>

#ifdef _MSC_VER
#include <windows.h>
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include "../include/llcopyfile.h"
#include "../include/lllogger.h"

llCopyFile::llCopyFile() : llWorker() {
	SetCommandName("CopyFile");

	move = 0;
}

int llCopyFile::Prepare(void) {

	return 1;
}

int llCopyFile::RegisterOptions(void) {
	if (!llWorker::RegisterOptions()) return 0;

	RegisterValue("-source", &source, LLWORKER_OBL_OPTION);
	RegisterValue("-target", &target, LLWORKER_OBL_OPTION);

	RegisterFlag("-move", &move);

	return 1;
}

int llCopyFile::Exec(void) {
	llWorker::Exec();

	if (move)
		MoveFileEx(source, target, 0);
	else
		CopyFile(source, target, 0);

	return 1;
}
