#ifndef _PLLQUADITERATOR_H_
#define _PLLQUADITERATOR_H_

#include "../include/llworker.h"
#include "../include/llmapworker.h"

class llQuadIterator : public llMapWorker {

protected:

	int   level, num_done;

	char myquadx[1000], myquady[1000];

public:

	llQuadIterator();

	llWorker * Clone() {
		return new llQuadIterator(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
