#ifndef _PLLINACTIVATEALLVERTICES_H_
#define _PLLINACTIVATEALLVERTICES_H_

#include "../include/llworker.h"
#include "../include/llmapworker.h"

class llInactivateAllVertices : public llMapWorker {

protected:


public:

	llInactivateAllVertices();

	llWorker * Clone() {
		return new llInactivateAllVertices(*this);
	}

	virtual int RegisterOptions(void);
	virtual int Exec(void);

};

#endif
