#ifndef _PLLSPLITAT_H_
#define _PLLSPLITAT_H_

#include <iostream>
#include "../include/lltrimod.h"

class llSplitAt : public llTriMod {

private:

	float x, y;

public:

	llSplitAt();

	virtual llWorker * Clone() {
		return new llSplitAt(*this);
	}

	int RegisterOptions(void);
	int Init(void);

};

#endif
