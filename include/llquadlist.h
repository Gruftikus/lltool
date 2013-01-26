#ifndef _PLLQUADLIST_H_
#define _PLLQUADLIST_H_

#include <math.h>
#include <vector>

#include "../include/lllogger.h"


class llQuad {

private:

    	unsigned int npoints, maxpoints;
    	int          has_sub_quads;
	llQuad *subquads[2][2];

	std::vector<int>   points;
	std::vector<float> points_x;
	std::vector<float> points_y;

public:

	void Print(void) {
		_llLogger()->WriteNextLine(-LOG_INFO, "x=%i y=%i x1=%f y1=%f x2=%f y2=%f", x, y, x1, y1, x2, y2);
	}

	float x1, y1, x2, y2; //corner coordinates
	int x, y; //current position in the 2d-grid

	//constructor
	llQuad();
	llQuad(int _x, int _y, float _x1, float _y1, float _x2, float _y2);

	int SetSubQuad(unsigned int _x, unsigned int _y, llQuad *_quad) {
		if (_x > 1 || _y > 1) return 0;
		subquads[_x][_y] = _quad;
		has_sub_quads = 1;
		return 1;
	}

	void SetMaxPoints(int _maxpoints) {
		maxpoints = _maxpoints;
	};

	int NPointsLeft(void) {
		return maxpoints - npoints;
	};

	int GetNumPoints(void) {
		return npoints;
	};

	void AddPoint(float _x, float _y, int _point_num) {
		if (_x < x1 || _x > x2 || _y < y1 || _y > y2) return;
		if (npoints == points.size()) {
			points.resize  (npoints + 1000);
			points_x.resize(npoints + 1000);
			points_y.resize(npoints + 1000);
		}
		points[npoints]   = _point_num;
		points_x[npoints] = _x;
		points_y[npoints] = _y;
		npoints++;
		if (has_sub_quads) {
			if (subquads[0][0]) subquads[0][0]->AddPoint(_x, _y, _point_num);
			if (subquads[0][1]) subquads[0][1]->AddPoint(_x, _y, _point_num);
			if (subquads[1][0]) subquads[1][0]->AddPoint(_x, _y, _point_num);
			if (subquads[1][1]) subquads[1][1]->AddPoint(_x, _y, _point_num);
		}
	};

	int GetPointNum(int _i) {
		return points[_i];
	}

	float GetPointX(int _i) {
		return (points_x[_i]);
	}
	float GetPointY(int _i) {
		return (points_y[_i]);
	}

	int GetPoint(float _x, float _y) {
		for (unsigned int i=0; i<points.size();i++) {
			if (fabs(points_x[i] - _x) < 1.f && fabs(points_y[i] - _y) < 1.f) return points[i];
		}
		return -1;
	}

	int GetMinDistance(float *_min, float _x, float _y, float _radius);

};

class llQuadList {

private:

	std::vector<llQuad*> v;
	std::vector<llQuadList*> subs;
	std::vector<llQuadList*> masters;
	std::vector<int>         pos;
	unsigned int pointer;
	llQuadList *subtree;

public:

	//constructor
	llQuadList();
	llQuadList(int _pos_x, int _pos_y, int _x, int _y, float _x1, float _y1, float _x2, float _y2);

	void SetMaxPoints(int _maxpoints) {
		for (unsigned int i=0; i<v.size(); i++) {
			v[i]->SetMaxPoints(_maxpoints);
		}
	};

	int AddPoint(float _x, float _y, int _num); 

	llQuad *AddQuad(int _p1, int _p2, float _x1, float _y1, float _x2, float _y2);

	void AddQuad(llQuad *_quad) {
		v.push_back(_quad);
		subs.push_back(NULL);
		masters.push_back(NULL);
		pos.push_back(0);
	}

	void AddMaster(llQuadList *_master) {
		masters[masters.size() - 1] = _master;
	}

	void AddPos(int _pos) {
		pos[pos.size() - 1] = _pos;
	}

#if 0
	void Reset(void) {
		pointer=0;
	};


	int GetNextQuad(void) {
		pointer++;
		if (pointer >= v.size()) return 0;
		return 1;
	};
#endif

	unsigned int GetNumQuads(void) {
		return v.size();
	};

	llQuad * GetQuad(float _x, float _y, int _num = -1);

	int GetQuads(llQuadList **_quads, float _x1, float _y1, float _x2, float _y2);

	llQuad * GetQuad(int _x, int _y);

	llQuad * GetQuad(unsigned int _n) {
		if (_n >= v.size()) return NULL;
		return v[_n];
	}

	int GetQuadNum(int _x, int _y);

	void SetSubQuads(llQuadList *_sub) {
		subtree = _sub;
	}

	llQuadList *GetSubQuads() {
		return subtree;
	}

	int SetSubQuadList(llQuadList *_sub, int _num) {
		if (_num < 0 || _num >= int(subs.size())) {
			return 0;
		}
		subs[_num] = _sub;
		return 1;
	}

	int AddSubQuad(int _num, llQuad *_quad, llQuadList **_ret = NULL, int *_pos = NULL) {
		if (_num < 0 || _num >= int(subs.size())) {
			//std::cout << "no quad" << std::endl;
			return 0;
		}
		if (!subs[_num]) {
			subs[_num] = new llQuadList();
			if (masters[_num]) masters[_num]->SetSubQuadList(subs[_num], pos[_num]);
		}
		if (_ret) *_ret = subs[_num];
		if (_pos) *_pos = subs[_num]->GetNumQuads();
		subs[_num]->AddQuad(_quad);
		return 1;
	}
	
	// void SubQuadLevels(int _levels);
#if 0
	int GetCurrentX(void) {
		return v[pointer]->x;
	}

	int GetCurrentY(void) {
		return v[pointer]->y;
	}
#endif
};

#endif
