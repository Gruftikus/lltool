#ifndef _PLLSELECTALL_H_
#define _PLLSELECTALL_H_

#include <iostream>
#include <windows.h>
#include "../include/llmapworker.h"
#include "../include/llutils.h"

class llSelectAll : public llMapWorker {

protected:

	
public:

	//constructor
	llSelectAll();

	virtual llWorker * Clone() {
		return new llSelectAll(*this);
	}

	virtual int RegisterOptions(void);
	virtual int Init(void);

};

#endif
