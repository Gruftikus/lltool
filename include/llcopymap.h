#ifndef _PLLCOPYMAP_H_
#define _PLLCOPYMAP_H_

#include "../include/llmapworker.h"
#include "../include/llmap.h"

class llCopyMap : public llMapWorker {

protected:

	char *targetname;
	int   dist;

public:

	llCopyMap();

	llWorker * Clone() {
		return new llCopyMap(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
