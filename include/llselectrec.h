#ifndef _PLLSELECTREC_H_
#define _PLLSELECTREC_H_

#include <iostream>
#include <windows.h>
#include "../include/llmapworker.h"
#include "../include/llutils.h"

class llSelectRec : public llWorker {

protected:

	float x1, x2, y1, y2;
	
public:

	//constructor
	llSelectRec();

	virtual llWorker * Clone() {
		return new llSelectRec(*this);
	}

	virtual int RegisterOptions(void);
	virtual int Init(void);

};

#endif
