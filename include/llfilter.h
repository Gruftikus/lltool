#ifndef _PLLFILTER_H_
#define _PLLFILTER_H_

#include "../include/llmapworker.h"
#include "../include/llmap.h"

class llFilter : public llMapWorker {

protected:

	char *targetname;
	int   dist;
	int   makeshort, overwrite, makederivatives, rgb;

public:

	llFilter();

	llWorker * Clone() {
		return new llFilter(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
