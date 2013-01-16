#ifndef _PLLSETGRIDBORDER_H_
#define _PLLSETGRIDBORDER_H_

#include "../include/llset.h"

class llSetGridBorder : public llSet {

private:

	float gridx, gridy, zmin;

public:

	llSetGridBorder();

	virtual llWorker * Clone() {
		return new llSetGridBorder(*this);
	}

	int RegisterOptions(void);
	int Exec(void);

};

#endif
