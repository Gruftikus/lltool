#ifndef _PLLSETPATH_H_
#define _PLLSETPATH_H_

#include <iostream>

#include "../include/llworker.h"
#include "../include/llutils.h"

class llSetPath : public llWorker {

protected:

	char *path;
	
public:

	//constructor
	llSetPath();

	virtual llWorker * Clone() {
		return new llSetPath(*this);
	}

	virtual int RegisterOptions(void);
	virtual int Init(void);

};

#endif
