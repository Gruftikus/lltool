#ifndef _PLLCREATENORMALMAP_H_
#define _PLLCREATENORMALMAP_H_

#include "../include/llmapworker.h"
#include "../include/llmap.h"

class llCreateNormalMap : public llMapWorker {

protected:

	char *targetname;
	int   lodshadows;
	float northboost, boost;
	
public:

	llCreateNormalMap();

	llWorker * Clone() {
		return new llCreateNormalMap(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
