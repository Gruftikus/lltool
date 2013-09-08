#ifndef _PLLACTIVATEFRACTIONOFVERTICES_H_
#define _PLLACTIVATEFRACTIONOFVERTICES_H_

#include "../include/llset.h"

class llActivateFractionOfVertices : public llSet {

private:

	float n;

public:

	llActivateFractionOfVertices();

	virtual llWorker * Clone() {
		return new llActivateFractionOfVertices(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
