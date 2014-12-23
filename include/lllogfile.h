#ifndef _PLLLOGFILE_H_
#define _PLLLOGFILE_H_

#include "llworker.h"

class llLogFile : public llWorker {

protected:

	FILE *file;
	char *filename;

public:

	llLogFile();

	virtual llWorker * Clone() {
		return new llLogFile(*this);
	}

	virtual int RegisterOptions(void);
	virtual int Exec(void);

};

#endif
