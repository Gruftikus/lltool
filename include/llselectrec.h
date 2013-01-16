#ifndef _PLLSELECTREC_H_
#define _PLLSELECTREC_H_

#include "../include/llmapworker.h"
#include "../include/llutils.h"

class llSelectRec : public llWorker {

protected:

	float x1, x2, y1, y2;
	
public:

	llSelectRec();

	virtual llWorker * Clone() {
		return new llSelectRec(*this);
	}

	int RegisterOptions(void);
	int Exec(void);

};

#endif
