#ifndef _PLLSETFLAG_H_
#define _PLLSETFLAG_H_

#include "../include/llworker.h"
#include "../include/llutils.h"

class llSetFlag : public llWorker {

protected:

	int   hidden, unselect, zerosuppression;
	char *name, *value;
	char *replace, *with;
	int   num;
	
public:

	llSetFlag();

	virtual llWorker * Clone() {
		return new llSetFlag(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
