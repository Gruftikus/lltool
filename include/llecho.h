#ifndef _PLLECHO_H_
#define _PLLECHO_H_

#include "../include/llmapworker.h"
#include "../include/llutils.h"

class llEcho : public llWorker {

protected:

	char *text;
	
public:

	llEcho();

	virtual llWorker * Clone() {
		return new llEcho(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
