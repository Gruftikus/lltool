#ifndef _PLLCELLITERATOR_H_
#define _PLLCELLITERATOR_H_

#include "../include/llworker.h"
#include "../include/llmapworker.h"

class llCellIterator : public llMapWorker {

protected:

	unsigned int num_done, n_total;
	int x_min, x_max, y_min, y_max;
	int running_x, running_y;
	float cellsize_x, cellsize_y;

public:

	llCellIterator();

	llWorker * Clone() {
		return new llCellIterator(*this);
	}

	int IsRepeatWorker() {
		return 1;
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
