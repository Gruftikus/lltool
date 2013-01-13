#ifndef _PLLSELECTALL_H_
#define _PLLSELECTALL_H_

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

	int RegisterOptions(void);
	int Init(void);

};

#endif
