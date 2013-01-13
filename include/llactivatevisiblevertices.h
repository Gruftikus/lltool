#ifndef _PLLACTIVATEVISIBLEVERTICES_H_
#define _PLLACTIVATEVISIBLEVERTICES_H_

#include "../include/lltrimod.h"

class llActivateVisibleVertices : public llTriMod {

private:

	float x, y, z, radius;

public:

	llActivateVisibleVertices();

	virtual llWorker * Clone() {
		return new llActivateVisibleVertices(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Init(void);

};

#endif
