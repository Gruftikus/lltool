#ifndef _PLLEXPORTMAP_H_
#define _PLLEXPORTMAP_H_

#include "../include/llworker.h"
#include "../include/llmapworker.h"

class llExportMap : public llMapWorker {

protected:

	char *filename;
	int  bits, compress;
	int  scale;
	int  flipx, flipy;

public:

	llExportMap();

	llWorker * Clone() {
		return new llExportMap(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
