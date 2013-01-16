#ifndef _PLLREADDATAFILE_H_
#define _PLLREADDATAFILE_H_

#include "../include/llset.h"

class llReadDataFile : public llSet {

private:

	char *filename;

public:

	llReadDataFile();

	virtual llWorker * Clone() {
		return new llReadDataFile(*this);
	}

	int RegisterOptions(void);
	int Exec(void);

};

#endif
