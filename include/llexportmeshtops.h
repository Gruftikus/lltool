#ifndef _PLLEXPORTMESHTOPS_H_
#define _PLLEXPORTMESHTOPS_H_

#include "../include/llworker.h"
#include "../include/lltrimod.h"

class llExportMeshToPs : public llTriMod {

protected:

	char *filename;

public:

	llExportMeshToPs();

	llWorker * Clone() {
		return new llExportMeshToPs(*this);
	}

	virtual int Prepare(void);
	virtual int RegisterOptions(void);
	virtual int Exec(void);

};

#endif
