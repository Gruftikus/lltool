#ifndef _PLLDELETEMAP_H_
#define _PLLDELETEMAP_H_

#include "../include/llmapworker.h"
#include "../include/llmap.h"

class llDeleteMap : public llMapWorker {

protected:

public:

	llDeleteMap();

	llWorker * Clone() {
		return new llDeleteMap(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
