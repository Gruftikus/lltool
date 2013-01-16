#ifndef _PLLSTENCILPOLYGON_H_
#define _PLLSTENCILPOLYGON_H_

#include "../include/lltrimod.h"

class llStencilPolygon : public llTriMod {

private:

	char *polygon_name;

public:

	llStencilPolygon();

	virtual llWorker * Clone() {
		return new llStencilPolygon(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
