#ifndef _PLLSPLITAT_H_
#define _PLLSPLITAT_H_

#include "../include/lltrimod.h"

class llSplitAt : public llTriMod {

private:

	float x, y;

public:

	llSplitAt();

	virtual llWorker * Clone() {
		return new llSplitAt(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
