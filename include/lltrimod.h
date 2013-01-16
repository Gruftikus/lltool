#ifndef _PLLTRIMOD_H_
#define _PLLTRIMOD_H_

#include "../include/llworker.h"
#include "../include/llmaplist.h"

class llTriMod : public llWorker {

protected:

    llMap          *map;
	char           *mapname;
	llPointList    *points;
	llTriangleList *triangles;


public:

    //constructor
    llTriMod();

	virtual llWorker * Clone() {
		std::cout << "I should never be here...." << std::endl;
		return new llWorker(*this);
	}

	virtual int Prepare(void);
	virtual int RegisterOptions(void);
	virtual int Exec(void);

};

#endif
