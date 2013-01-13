#ifndef _PLLDIAMONDSQUARE_H_
#define _PLLDIAMONDSQUARE_H_

#include "../include/llmapworker.h"
#include "../include/llmap.h"

class llDiamondSquare : public llMapWorker {

protected:

	float range, offset;
	char *alg_list;

public:

	llDiamondSquare();

	llWorker * Clone() {
		return new llDiamondSquare(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Init(void);

};

#endif
