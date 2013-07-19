#ifndef _PLLLINELIST_H_
#define _PLLLINELIST_H_

#include <vector>

#include "../include/llpointlist.h"
#include "../include/llmap.h"
#include "../include/lllogger.h"

class llLine {

private:

	int p1, p2;
	llPointList *points;
	
public:

	//constructor
	llLine(int _n1, int _n2, llPointList *_r);
	llLine();

	float GetX1() {
		return points->GetX(p1);
	}
	float GetX2() {
		return points->GetX(p2);
	}
	float GetY1() {
		return points->GetY(p1);
	}
	float GetY2() {
		return points->GetY(p2);
	}

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

	int GetN(void) {
		return counter;
	}

	llLine *GetLine(int _n) {
		return &(v[_n]);
	}

};

#endif
