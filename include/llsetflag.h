#ifndef _PLLSETFLAG_H_
#define _PLLSETFLAG_H_

#include "../include/llmapworker.h"
#include "../include/llutils.h"

class llSetFlag : public llWorker {

protected:

	int hidden, unselect;
	char *name, *value;
	
public:

	llSetFlag();

	virtual llWorker * Clone() {
		return new llSetFlag(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Init(void);

};

#endif
