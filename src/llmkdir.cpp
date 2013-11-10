#include <stdlib.h>
#include <direct.h>

#include "../include/llmkdir.h"
#include "../include/lllogger.h"

#define MKDIR(a)	_mkdir(a)

#ifdef _MSC_VER
#define DELIM '\\'
#else
#define DELIM '/'
#endif

llMkDir::llMkDir() : llWorker() {
	SetCommandName("MkDir");
}

int llMkDir::Prepare(void) {

	hasname = 0;

	return 1;
}

int llMkDir::RegisterOptions(void) {
	if (!llWorker::RegisterOptions()) return 0;

	RegisterValue("-path",        &path, LLWORKER_OBL_OPTION);
	RegisterFlag ("-hasfilename", &hasname);

	return 1;
}

int llMkDir::Exec(void) {
	llWorker::Exec();

	int total_length = strlen(path);

	for (int i=0; i<total_length; i++) {
		if (path[i] == DELIM) {
			path[i] = '\0';
			MKDIR(path);
			path[i] = DELIM;
		}
	}

	if (!hasname) MKDIR(path);

	return 1;
}
