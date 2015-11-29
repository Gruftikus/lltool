#include "../include/llquadlist.h"

llQuad::llQuad(int _x, int _y, float _x1, float _y1, float _x2, float _y2) {

    x  = _x;
    y  = _y;
	x1 = _x1;
	y1 = _y1;
	x2 = _x2;
	y2 = _y2;
	
	if (x1 > x2 || y1 > y2) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "llQuad: Order of coordinates not correct");
	} 

    npoints   = 0;
    maxpoints = 0x7FFFFFFF;

	subquads[0][0] = NULL;
	subquads[1][0] = NULL;
	subquads[0][1] = NULL;
	subquads[1][1] = NULL;
	has_sub_quads  = 0;
}

llQuad::llQuad() {
	subquads[0][0] = NULL;
	subquads[1][0] = NULL;
	subquads[0][1] = NULL;
	subquads[1][1] = NULL;
	has_sub_quads  = 0;
}

int llQuad::GetMinDistance(float *_min, float _x, float _y, float _radius) {
	//std::cout  << _x << ":" << _y << std::endl;
	if (_radius > 0.f && (_x+_radius < x1 || _x-_radius > x2 || _y+_radius < y1 || _y-_radius > y2)) {
		//no hope that the circle will ever touch this quad
		return 0;
	} 
	for (unsigned int j=0; j < points.size(); j++) {
		//std::cout << _x << ":" << _y << " --> " << points_x[j] << ":" << points_y[j] << std::endl;
		float my_x = _x - points_x[j];
		float my_y = _y - points_y[j];
		my_x *= my_x;
		my_y *= my_y;
		float minnew = my_x + my_y;

		if (minnew < *_min) {
			//std::cout  << _x << ":" << _y << " --> " << minnew << std::endl;
			*_min = minnew;
		}
	}
	return 1;
}

//constructor
llQuadList::llQuadList() {
    v.resize(0);
	subs.resize(0);
	masters.resize(0);
	pos.resize(0);
	pointer = 0;
	subtree = NULL;
} 

llQuadList::llQuadList(int _pos_x, int _pos_y, int _x, int _y, float _x1, float _y1, float _x2, float _y2) {
	v.resize(0);
	subs.resize(0);
	masters.resize(0);
	pos.resize(0);
	pointer = 0;
	subtree = NULL;

	for (int ix=_pos_x; ix < _pos_x+_x; ix++) {
		for (int iy=_pos_y; iy < _pos_y+_y; iy++) {
			AddQuad(ix, iy, float(ix)*(_x2-_x1)/float(_x),
				float(iy)*(_y2-_y1)/float(_y),
				float(ix+1)*(_x2-_x1)/float(_x),
				float(iy+1)*(_y2-_y1)/float(_y));
		}
	}
} 

llQuad *llQuadList::AddQuad(int _p1, int _p2, float _x1, float _y1, float _x2, float _y2) {
	//std::cout << _x1 << ":" << _y1 << ":" << _x2 << ":" << _y2 << std::endl;
	llQuad *newquad = new llQuad(_p1, _p2, _x1, _y1, _x2, _y2);
	v.push_back(newquad);
	subs.push_back(NULL);
	masters.push_back(NULL);
	pos.push_back(0);
	return newquad;
}

llQuad *llQuadList::GetQuad(float _x, float _y, int _num) {

	int num = _num;
	if (_num < 0) {
		num = 0;
	}
	for (unsigned int i=0; i<v.size(); i++) {
		if (v[i]->x1-0.5f <= _x && v[i]->x2+0.5f >= _x && v[i]->y1-0.5f <= _y && v[i]->y2+0.5f >= _y) {
			if (!num)
				return v[i];
			else 
				num--;
		}
	}
	if (_num < 0) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "Quad not found (x=%f ,y=%f)", _x, _y);
	}
	return NULL;
};

int llQuadList::GetQuads(llQuadList **_quads, float _x1, float _y1, float _x2, float _y2) {
	//generates a list of quads which have a cross section with a rectangle
	for (unsigned int i=0; i<v.size(); i++) {
		//std::cout << "check quad: " << std::endl;
		//v[i]->Print();
		if (!(v[i]->x2 <= _x1 || v[i]->x1 >= _x2 || v[i]->y2 <= _y1 || v[i]->y1 >= _y2)) {  //not out of range
			if (v[i]->x1 >= _x1 && v[i]->x2 <= _x2 && v[i]->y1 >= _y1 && v[i]->y2 <= _y2) {  //fully covered quad
				//std::cout << "fully covered" << std::endl;
				(*_quads)->AddQuad(v[i]);
				//v[i]->Print();
			} else if (subs[i]) {
				//std::cout << "subs: " << std::endl;
				subs[i]->GetQuads(_quads, _x1, _y1, _x2, _y2);
			} else {
				(*_quads)->AddQuad(v[i]);
				//v[i]->Print();
				//std::cout << "no sub" << std::endl;
			}
		}
	}
	return 1;
}

llQuad *llQuadList::GetQuad(int _x, int _y) {
	for (unsigned int i=0; i<v.size(); i++) {
		if (v[i]->x == _x && v[i]->y == _y) {
			return v[i];
		}
	}
	_llLogger()->WriteNextLine(-LOG_ERROR, "Quad not found (x=%i ,y=%i)", _x, _y);
	return NULL;
}

int llQuadList::GetQuadNum(int _x, int _y) {
	for (unsigned int i=0; i<v.size(); i++) {
		if (v[i]->x == _x && v[i]->y == _y) {
			return i;
		}
	}
	_llLogger()->WriteNextLine(-LOG_ERROR, "Quad not found (x=%i ,y=%i)", _x, _y);
	return -1;
}

#if 0
void llQuadList::SubQuadLevels(int _levels) {
	if (!_levels) return;
	if (!subtree)
		subtree = new (std::nothrow) llQuadList(mesg, v.size()*4);
	if (subtree) {
		subtree->SubQuadLevels(_levels - 1);
	} else {
		mesg->WriteNextLine(-LOG_FATAL, "Allocation of subtree failed (out of memory?)");
	}
}
#endif

int llQuadList::AddPoint(float _x, float _y, int _num) {
	llQuad * myquad = GetQuad(_x, _y, 0);
	int num = -1;  //first try, must return a quad
	while (myquad) {
		if (myquad->NPointsLeft() < 0) 
			return 0;
		else {
			myquad->AddPoint(_x, _y, _num); //for statistics and fast quadtree search
			num++;
			myquad = GetQuad(_x, _y, num);
		}
	} 
	//if (subtree) return subtree->AddPoint(_x, _y, _num);
	return 1;
};