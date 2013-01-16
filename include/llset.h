#ifndef _PLLSET_H_
#define _PLLSET_H_

#include "../include/llworker.h"
#include "../include/llmaplist.h"

class llSet : public llWorker {

protected:

    llMap       *map;
	char        *mapname;
	llPointList *points;

public:
    
    llSet();

	virtual llWorker * Clone() {
		std::cout << "I should never be here...." << std::endl;
		return new llWorker(*this);
	}

	virtual int Prepare(void);
	virtual int RegisterOptions(void);
	virtual int Exec(void);

};

#endif
