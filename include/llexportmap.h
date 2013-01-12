#ifndef _PLLEXPORTMAP_H_
#define _PLLEXPORTMAP_H_

#include <iostream>
#include "../include/llworker.h"
#include "../include/llmapworker.h"

class llExportMap : public llMapWorker {

protected:

	char *filename;
	int  bits, compress;

public:

	llExportMap();

	llWorker * Clone() {
		return new llExportMap(*this);
	}

	virtual int RegisterOptions(void);
	virtual int Init(void);

};

#endif
