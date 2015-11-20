#ifndef _PLLEXPORTMESHTOOBJ_H_
#define _PLLEXPORTMESHTOOBJ_H_

#include "../include/llworker.h"
#include "../include/lltrimod.h"

class llExportMeshToObj : public llTriMod {

protected:

	char *filename;
	char *texname;
	char *texmap;
	char *mtlname;
	int   createpedestals, flipu, flipv, sort, nosplit;
	float trans_x, trans_y, trans_z, scale, pedestalz, pedestaldiff;

	llPointList    *newpoints;
	llTriangleList *newtriangles;
	int MakeSelection();

	float xmin, xmax, ymin, ymax, zmin, zmax;

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
