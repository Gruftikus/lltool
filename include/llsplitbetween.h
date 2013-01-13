#ifndef _PLLSPLITBETWEEN_H_
#define _PLLSPLITBETWEEN_H_

#include "../include/lltrimod.h"

class llSplitBetween : public llTriMod {

private:

	float x1, y1, x2, y2;

public:

	llSplitBetween();

	virtual llWorker * Clone() {
		return new llSplitBetween(*this);
	}

	int RegisterOptions(void);
	int Init(void);

};

#endif
