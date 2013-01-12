#ifndef _PLLTRIMOD_H_
#define _PLLTRIMOD_H_

#include <iostream>
#include "../include/llworker.h"
#include "../include/llmaplist.h"

class llTriMod : public llWorker {

protected:

    llMap          *heightmap;
	char           *map;
	llPointList    *points;
	llTriangleList *triangles;


public:

    //constructor
    llTriMod();

	virtual llWorker * Clone() {
		std::cout << "I should never be here...." << std::endl;
		return new llWorker(*this);
	}

	virtual int RegisterOptions(void);
	virtual int Init(void);

};

#endif
