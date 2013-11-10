#ifndef _PLLMKDIR_H_
#define _PLLMKDIR_H_

#include "llworker.h"

class llMkDir : public llWorker {

protected:

	char *path;
	int   hasname;

public:

	llMkDir();

	virtual llWorker * Clone() {
		return new llMkDir(*this);
	}

	virtual int Prepare(void);
	virtual int RegisterOptions(void);
	virtual int Exec(void);

};

#endif
