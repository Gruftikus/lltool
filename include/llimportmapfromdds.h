#ifndef _PLLIMPORTMAPFROMDDS_H_
#define _PLLIMPORTMAPFROMDDS_H_

#include "../include/llworker.h"
#include "../include/llmap.h"

class llImportMapFromDDS : public llWorker {

protected:

	float x1, y1, x2, y2, z;
	char *mapname;
	char *filename;
	int   even;
	int   rgb;
	int   replacemap;
	int   nogeometry;

public:

	llImportMapFromDDS();

	llWorker * Clone() {
		return new llImportMapFromDDS(*this);
	}

	virtual int Prepare(void);
	virtual int RegisterOptions(void);
	virtual int Exec(void);

};

#endif
