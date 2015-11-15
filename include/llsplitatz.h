#ifndef _PLLSPLITATZ_H_
#define _PLLSPLITATZ_H_

#include "../include/lltrimod.h"

class llSplitAtZ : public llTriMod {

private:

	float z;

public:

	llSplitAtZ();

	virtual llWorker * Clone() {
		return new llSplitAtZ(*this);
	}

	int RegisterOptions(void);
	int Exec(void);

};

#endif
