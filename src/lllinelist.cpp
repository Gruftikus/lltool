#include "../include/lllinelist.h"

llLine::llLine(int _n1, int _n2, llPointList *_r) {

	p1     = _n1;
	p2     = _n2;
	points = _r;
}

llLine::llLine() {
	p1     = -1;
	p2     = -1;
}

llLineList::llLineList(int _n, llPointList *_x, llMap *_map) {

	v.resize(_n + 1);
	counter = 0;
	points  = _x;
	map     = _map;
	mesg = _llLogger();
} 

int llLineList::AddLine(int _p1, int _p2) {

	if (_p1 == _p2) {
		mesg->WriteNextLine(LOG_WARNING, "Line with two equal points (maybe a very small one) -> removed");
		mesg->Dump();
		return -1;
	}

	v[counter] = llLine(_p1, _p2, points);
	counter++;

	if (counter == v.size()) v.resize(counter+100);
	return counter - 1;
}

int llLineList::AddLine(float _x1, float _y1, float _x2, float _y2) {

	int point1 = points->GetPoint(_x1, _y1);
	if (point1 < 0)
		point1 = points->AddPoint(_x1, _y1, map->GetZ(_x1, _y1));
	int point2 = points->GetPoint(_x2, _y2);
	if (point2 < 0)
		point2 = points->AddPoint(_x2, _y2, map->GetZ(_x2, _y2));

	return AddLine(point1, point2);

}
