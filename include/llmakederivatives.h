#ifndef _PLLMAKEDERIVATIVES_H_
#define _PLLMAKEDERIVATIVES_H_

#include "../include/llmapworker.h"
#include "../include/llmap.h"

class llMakeDerivatives : public llMapWorker {

protected:

	int   makeshort;

	float x1max;
	float y1max;
	float x2max;
	float y2max;

public:

	llMakeDerivatives();

	llWorker * Clone() {
		return new llMakeDerivatives(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Init(void);

};

#endif
