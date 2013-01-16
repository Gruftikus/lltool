#ifndef _PLLFILLCOLORMAP_H_
#define _PLLFILLCOLORMAP_H_

#include "../include/llmapworker.h"
#include "../include/llmap.h"

class llFillColorMap : public llMapWorker {

protected:

	char *alg_list_red, *alg_list_blue, *alg_list_green, *alg_list_alpha;

public:

	llFillColorMap();

	llWorker * Clone() {
		return new llFillColorMap(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
