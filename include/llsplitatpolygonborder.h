#ifndef _PLLSPLITATPOLYGONBORDER_H_
#define _PLLSPLITATPOLYGONBORDER_H_

#include <iostream>
#include "../include/lltrimod.h"

class llSplitAtPolygonBorder : public llTriMod {

private:

	char *polygon_name;

public:

	llSplitAtPolygonBorder();

	virtual llWorker * Clone() {
		return new llSplitAtPolygonBorder(*this);
	}

	int RegisterOptions(void);
	int Init(void);

};

#endif
