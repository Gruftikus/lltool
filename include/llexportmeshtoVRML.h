#ifndef _PLLEXPORTMESHTOVRML_H_
#define _PLLEXPORTMESHTOVRML_H_

#include "llexportmeshtoobj.h"

class llExportMeshToVRML : public llExportMeshToObj {

protected:



public:

	llExportMeshToVRML();

	llWorker *Clone() {
		return new llExportMeshToVRML(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
