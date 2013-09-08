#ifndef _PLLREMOVEINACTIVEVERTICES_H_
#define _PLLREMOVEINACTIVEVERTICES_H_

#include "../include/llset.h"

class llRemoveInactiveVertices : public llSet {

private:


public:

	llRemoveInactiveVertices();

	virtual llWorker * Clone() {
		return new llRemoveInactiveVertices(*this);
	}

	int RegisterOptions(void);
	int Exec(void);

};

#endif
