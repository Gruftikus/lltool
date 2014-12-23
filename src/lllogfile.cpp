#include <stdlib.h>

#include "../include/lllogfile.h"
#include "../include/lllogger.h"


llLogFile::llLogFile() : llWorker() {
	SetCommandName("LogFile");

	file     = NULL;
	filename = NULL;
}

int llLogFile::RegisterOptions(void) {
	if (!llWorker::RegisterOptions()) return 0;

	RegisterValue("-filename", &filename, LLWORKER_OBL_OPTION);

	return 1;
}

int llLogFile::Exec(void) {
	llWorker::Exec();

	if (file) fclose(file);

	file = fopen (filename, "w");
	if (!file) {   
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: could not open %s", command_name, filename);
		return 0;
    }

	_llLogger()->SetLogFile(file);

	return 1;
}