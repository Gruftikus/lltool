#ifndef _PLLLINELIST_H_
#define _PLLLINELIST_H_

#include <vector>

#include "../include/llpointlist.h"
#include "../include/llmap.h"
#include "../include/lllogger.h"

class llLine {

private:

	int p1, p2;
	llPointList * points;
	
public:

	//constructor
	llLine(int _n1, int _n2, llPointList *_r);
	llLine();

};

class llLineList {

private:

	std::vector<llLine> v;
	unsigned int counter;
	llPointList *points;
	llMap       *map;
	llLogger    *mesg;


public:

	//constructor
	llLineList(int _n, llPointList *_x, llMap *_map);

	int AddLine(int _p1, int _p2);
	int AddLine(float _x1, float _y1, float _x2, float _y2);

};

#endif
