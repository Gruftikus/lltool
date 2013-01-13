#ifndef _PLLSPLITATGRID_H_
#define _PLLSPLITATGRID_H_

#include "../include/lltrimod.h"

class llSplitAtGrid : public llTriMod {

private:

	float gridx, gridy;

public:

	llSplitAtGrid();

	virtual llWorker * Clone() {
		return new llSplitAtGrid(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Init(void);

};

#endif
