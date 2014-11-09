#ifndef _PLLSETCONTOUR_H_
#define _PLLSETCONTOUR_H_

#include "../include/llset.h"

class llSetContour : public llSet {

private:

	float gridx, gridy, z, offsetx, offsety, minmaxgrid;
	int   findmin, findmax, linear, onlyintracell;
	int   setmin, setmax;

public:

	llSetContour();

	virtual llWorker * Clone() {
		return new llSetContour(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
