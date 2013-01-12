

#include "..\include\llexit.h"
#include "..\include\lllogger.h"

#include <string.h>
#include <stdio.h>


//constructor
llExit::llExit() : llWorker() {

	SetCommandName("Exit");

}

int llExit::RegisterOptions(void) {
	if (!llWorker::RegisterOptions()) return 0;
	return 1;
}

int llExit::Init(void) {
	llWorker::Init();

	_llLogger()->Dump();
	_llLogger()->WriteNextLine(-LOG_COMMAND,"%s: Bye...", command_name);

	exit(1);

	return 1;
}
