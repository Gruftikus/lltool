#ifndef _PLLMAPWORKER_H_
#define _PLLMAPWORKER_H_

#include "../include/llworker.h"
#include "../include/llmaplist.h"
#include "../include/lltrianglelist.h"
#include "../include/llpolygonlist.h"

class llMapWorker : public llWorker {

protected:

    llMap          *map;
	char           *mapname;
	llPointList    *points;
	llTriangleList *triangles;
	llPolygonList  *polygons;


public:

    
    //constructor
    llMapWorker();

	virtual llWorker * Clone() {
		std::cout << "I should never be here...." << std::endl;
		return new llWorker(*this);
	}

	virtual int Prepare(void);
	virtual int RegisterOptions(void);
	virtual int Init(void);

};

#endif
