#ifndef _PLLEXPORTMAPTODDS_H_
#define _PLLEXPORTMAPTODDS_H_

#include "../include/llworker.h"
#include "../include/llmapworker.h"

class llExportMapToDDS : public llMapWorker {

protected:

	char *filename;
	int   scale;
	int   flipx, flipy;

	float bitrate;
	int   quality_level;
	int   noAdaptiveBlocks;
	int   makemips;
	char *fmt_string;



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
