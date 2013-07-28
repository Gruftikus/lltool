#ifndef _PLLEXPORTMESHTOOBJ_H_
#define _PLLEXPORTMESHTOOBJ_H_

#include "../include/llworker.h"
#include "../include/lltrimod.h"

class llExportMeshToObj : public llTriMod {

protected:

	char *filename;
	char *texname;
	char *mtlname;
	int   createpedestals;
	float trans_x, trans_y, trans_z, scale;

	llPointList    *newpoints;
	llTriangleList *newtriangles;
	int MakeSelection();

public:

	llExportMeshToObj();

	llWorker *Clone() {
		return new llExportMeshToObj(*this);
	}

	virtual int Prepare(void);
	virtual int RegisterOptions(void);
	virtual int Exec(void);

};

#endif
