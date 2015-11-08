#include "../include/llmatchregex.h"

#include <windows.h>
#include <tchar.h>

#include <regex>

llMatchRegEx::llMatchRegEx() : llWorker() {
	SetCommandName("MatchRegEx");
}


int llMatchRegEx::Prepare(void) {
	if (!llWorker::Prepare()) return 0;

	input = regex = NULL;
	not = 0;

	return 1;
}

int llMatchRegEx::RegisterOptions(void) {
	if (!llWorker::RegisterOptions()) return 0;

	RegisterValue("-input", &input, LLWORKER_OBL_OPTION);
	RegisterValue("-regex", &regex, LLWORKER_OBL_OPTION);

	RegisterFlag("-invert", &not);

	return 1;
}

int llMatchRegEx::Exec(void) {
	if (!llWorker::Exec()) return 0;

	repeat_worker = false;
	
	const std::tr1::regex pattern(regex, std::tr1::regex_constants::icase);
	if (std::tr1::regex_match(input, pattern)) {
		return 1-not;
	}
	
	return not;
}
