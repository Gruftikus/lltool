#ifndef _PLLEXPORTMAPTODDS_H_
#define _PLLEXPORTMAPTODDS_H_

#include "../include/llworker.h"
#include "../include/llmapworker.h"

class llExportMapToDDS : public llMapWorker {

protected:

	char *filename;
	int  scale;


public:

	llExportMapToDDS();

	llWorker * Clone() {
		return new llExportMapToDDS(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
