#ifndef _PLLSORTTRIANGLES_H_
#define _PLLSORTTRIANGLES_H_

#include "../include/lltrimod.h"

class llSortTriangles : public llTriMod {

private:

	float gridx, gridy;
	int yx;

public:

	llSortTriangles();

	virtual llWorker * Clone() {
		return new llSortTriangles(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
