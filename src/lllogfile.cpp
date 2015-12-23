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

	RegisterValue("-filename", &filename);
	RegisterValue("-value", &filename); //compatible with old MPGUI syntax

	return 1;
}

int llLogFile::Exec(void) {
	llWorker::Exec();

	if (!filename) {   
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: no filename", command_name);
		return 0;
    }

	if (file) fclose(file);

	file = fopen (filename, "w");
	if (!file) {   
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: could not open %s", command_name, filename);
		return 0;
    }

	_llLogger()->SetLogFile(file);

	return 1;
}
