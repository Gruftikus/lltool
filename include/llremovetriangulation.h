#ifndef _PLLREMOVETRIANGULATION_H_
#define _PLLREMOVETRIANGULATION_H_

#include "../include/lltrimod.h"

class llRemoveTriangulation : public llTriMod {

private:

public:

	llRemoveTriangulation();

	virtual llWorker * Clone() {
		return new llRemoveTriangulation(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
