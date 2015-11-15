#ifndef _PLLGETHEIGHT_H_
#define _PLLGETHEIGHT_H_

#include <iostream>
#include "../include/llworker.h"
#include "../include/llmapworker.h"

class llGetHeight : public llMapWorker {

protected:

	float x, y;

public:

	llGetHeight();

	llWorker * Clone() {
		return new llGetHeight(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
