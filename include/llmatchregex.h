#ifndef _PLLMATCHREGEX_H_
#define _PLLMATCHREGEX_H_

#include <windows.h>
#include <tchar.h>

#include "../include/llworker.h"

class llMatchRegEx : public llWorker {

protected:

	char *input, *regex;
	int   not;

public:

	llMatchRegEx();

	llWorker * Clone() {
		return new llMatchRegEx(*this);
	}

	int IsRepeatWorker() {
		return 1;
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
