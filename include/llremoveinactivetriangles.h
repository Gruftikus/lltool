#ifndef _PLLREMOVEINACTIVETRIANGLES_H_
#define _PLLREMOVEINACTIVETRIANGLES_H_

#include "../include/lltrimod.h"

class llRemoveInactiveTriangles : public llTriMod {

private:


public:

	llRemoveInactiveTriangles();

	virtual llWorker * Clone() {
		return new llRemoveInactiveTriangles(*this);
	}

	int RegisterOptions(void);
	int Init(void);

};

#endif
