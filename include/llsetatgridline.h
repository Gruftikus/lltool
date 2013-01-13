#ifndef _PLLSETATGRIDLINE_H_
#define _PLLSETATGRIDLINE_H_

#include "../include/llset.h"

class llSetAtGridLine : public llSet {

private:

	float gridx, gridy, zmin, max;

public:

	llSetAtGridLine();

	virtual llWorker * Clone() {
		return new llSetAtGridLine(*this);
	}

	int RegisterOptions(void);
	int Init(void);

};

#endif
