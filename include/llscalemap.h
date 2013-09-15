#ifndef _PLLSCALEMAP_H_
#define _PLLSCALEMAP_H_

#include "../include/llmapworker.h"
#include "../include/llmap.h"

class llScaleMap : public llMapWorker {

protected:

	char *targetname;
	float factor, sizex, sizey;
	int   rgb;
	float source_gamma;

	char *pFilter;

public:

	llScaleMap();

	llWorker * Clone() {
		return new llScaleMap(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
