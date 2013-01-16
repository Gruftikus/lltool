#ifndef _PLLEXPORTMAPTOOBJ_H_
#define _PLLEXPORTMAPTOOBJ_H_

#include "../include/llworker.h"
#include "../include/lltrimod.h"

class llExportMapToObj : public llTriMod {

protected:

	char *filename;
	char *texname;
	char *mtlname;
	int   createpedestals;
	float trans_x, trans_y, trans_z;

public:

	llExportMapToObj();

	llWorker * Clone() {
		return new llExportMapToObj(*this);
	}

	virtual int Prepare(void);
	virtual int RegisterOptions(void);
	virtual int Exec(void);

};

#endif
