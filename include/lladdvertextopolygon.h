#ifndef _PLLADDVERTEXTOPOLYGON_H_
#define _PLLADDVERTEXTOPOLYGON_H_

#include "../include/llset.h"

class llAddVertexToPolygon : public llSet {

private:

	float x1, y1;
	char  *polygon_name;

public:

	llAddVertexToPolygon();

	virtual llWorker * Clone() {
		return new llAddVertexToPolygon(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
