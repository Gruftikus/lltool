#ifndef _PLLSETALGVERTICES_H_
#define _PLLSETALGVERTICES_H_

#include "../include/llset.h"

class llSetAlgVertices : public llSet {

private:

	int    max, nmax;
	double cutoff;
	char  *alg_list;

public:

	llSetAlgVertices();

	virtual llWorker * Clone() {
		return new llSetAlgVertices(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
