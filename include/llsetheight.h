#ifndef _PLLSETHEIGHT_H_
#define _PLLSETHEIGHT_H_

#include <iostream>
#include "../include/llworker.h"
#include "../include/llmapworker.h"

class llSetHeight : public llMapWorker {

protected:

	int   usegameunits;
	float zmin;

public:

	llSetHeight();

	llWorker * Clone() {
		return new llSetHeight(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Init(void);

};

#endif
