#ifndef _PLLTRIANGULATION_H_
#define _PLLTRIANGULATION_H_

#include "../include/llworker.h"
#include "../include/llmapworker.h"

class llTriangulation : public llMapWorker {

protected:


public:

	llTriangulation();

	llWorker * Clone() {
		return new llTriangulation(*this);
	}

	virtual int RegisterOptions(void);
	virtual int Init(void);

};

#endif
