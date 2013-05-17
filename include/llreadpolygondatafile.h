#ifndef _PLLREADPOLYGONDATAFILE_H_
#define _PLLREADPOLYGONDATAFILE_H_

#include "../include/llset.h"

class llReadPolygonDataFile : public llSet {

private:

	char *polygon_name, *filename;
	float scale_x, scale_y;
	float trans_x, trans_y;

public:

	llReadPolygonDataFile();

	virtual llWorker * Clone() {
		return new llReadPolygonDataFile(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
