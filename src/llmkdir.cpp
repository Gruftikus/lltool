#include <stdlib.h>

#ifdef _MSC_VER
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include "../include/llmkdir.h"
#include "../include/lllogger.h"

#ifdef _MSC_VER
#define MKDIR(a)	_mkdir(a)
#define DELIM '\\'
#else
#define MKDIR(a)	mkdir(a, 666)
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
