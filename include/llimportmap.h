#ifndef _PLLIMPORTMAP_H_
#define _PLLIMPORTMAP_H_

#include "../include/llworker.h"
#include "../include/llmap.h"

class llImportMap : public llWorker {

protected:

	float x1, y1, x2, y2, z;
	char *mapname;
	char *filename;
	int   even;
	int   rgb;

public:

	llImportMap();

	llWorker * Clone() {
		return new llImportMap(*this);
	}

	virtual int Prepare(void);
	virtual int RegisterOptions(void);
	virtual int Exec(void);

};

#endif
