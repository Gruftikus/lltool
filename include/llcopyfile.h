#ifndef _PLLCOPYFILE_H_
#define _PLLCOPYFILE_H_

#include "llworker.h"

class llCopyFile : public llWorker {

protected:

	char *source;
	char *target;

	int   move;

public:

	llCopyFile();

	virtual llWorker * Clone() {
		return new llCopyFile(*this);
	}

	virtual int Prepare(void);
	virtual int RegisterOptions(void);
	virtual int Exec(void);

};

#endif
