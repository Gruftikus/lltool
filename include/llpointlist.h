#ifndef _PLLPOINTLIST_H_
#define _PLLPOINTLIST_H_

#include <math.h>
#include <ANN/ANN.h>

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

#include "../include/llquadlist.h"

//Never change something in the basic types, because of the cast in tes4ll::llexportmeshtonif
class llCoord {

public:

	float u; 
	float v; 

	llCoord () {
		u = 0.f;
		v = 0.f;
	}

	llCoord (float _u, float _v) {
		u = _u;
		v = _v;
	}
};

struct llVector3 {

public:

	float x; 
	float y; 
	float z; 

	llVector3 () {
		x = 0.f;
		y = 0.f;
		z = 0.f;
	}

	llVector3 (float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

};

class llPointList {

private:

	std::vector<llVector3> v;
	std::vector<int>       active;
	std::vector<int>       tmp_active;
	std::vector<int>       flag;
	std::vector<int>       secondary;
	std::vector<double>    angle1;
	std::vector<double>    angle2;
	std::vector<llCoord>   uv;
	unsigned int counter;

	float tex00x, tex00y, tex11x, tex11y; //coords for the tex 
	llQuadList * quads;

	ANNkd_tree         *kdTree;
	ANNpointArray		dataPts;				// data points
	ANNpoint			queryPt;				// query point
	ANNidxArray			nnIdx;					// near neighbor indices
	ANNdistArray		dists;					// near neighbor distances


public:

	//constructor
	llPointList(int _n, llQuadList *_quads);
	//destructor
	~llPointList();

	int  AddPoint(float _x, float _y, float _z);
	int  GetPoint(float _x, float _y);
	int  GetPoint(float _x, float _y, float _z);
	
	llQuadList *GetQuadList() {
		return quads;
	};

	void Translation(float _x, float _y, float _z) {
		for (unsigned int i=0; i<counter; i++) {
			v[i].x += _x;
			v[i].y += _y;
			v[i].z += _z;
		}
	}

	void Scale(float _scale) {
		for (unsigned int i=0; i<counter; i++) {
			v[i].x *= _scale;
			v[i].y *= _scale;
			v[i].z *= _scale;
		}
	}

	std::vector<llVector3> GetVertices(void) {
		return v;
	}

	std::vector<llCoord> GetUV(void) {
		return uv;
	}

	void ClearSecondaryList(void) {
		for (unsigned int i=0; i<secondary.size(); i++) {
			secondary[i] = -1;
		}
	}

	void SetSecondary(int _a, int _b) {
		secondary[_a] = _b;
	}
	
	int GetSecondary(int _a) {
		return secondary[_a];
	}

	void SetInactive(int _a) {
		active[_a] = 0;
	}

	void SetActive(int _a) {
		active[_a] = 1;
	}

	int GetActive(int _a) {
		return active[_a];
	}

	void SetTmpInactive(int _a) {
		tmp_active[_a] = 0;
	}

	void ClearTmpInactive(void) {
		for (unsigned int i=0;i<counter;i++) {
			active[i] |= tmp_active[i];
			tmp_active[i] = 1;
		}
	}

	int RemoveInactiveVertices(void);

	void Resize(void) {
		v.resize(counter);
		uv.resize(counter);
		secondary.resize(counter);
		active.resize(counter);
		tmp_active.resize(counter);
		flag.resize(counter);
		angle1.resize(counter);
		angle2.resize(counter);
	}

	void Resize(int _i) {
		v.resize(_i);
		uv.resize(_i);
		secondary.resize(_i);
		active.resize(_i);
		tmp_active.resize(_i);
		flag.resize(_i);
		angle1.resize(_i);
		angle2.resize(_i);
	}

	void SetFlag(int _i, int _flag) {
		if (_i<0) return;
		flag[_i]=_flag;
	}

	int GetFlag(int _i) {
		if (_i < 0) return 0;
		return flag[_i];
	}

	int GetN(void) {
		return counter;
	}

	int GetN(float _x1, float _y1, float _x2, float _y2);

	float GetX(int _n) {
		return v[_n].x;
	}

	float GetY(int _n) {
		return v[_n].y;
	}

	float GetZ(int _n) {
		return v[_n].z;
	}

	float GetU(int _n) {
		return uv[_n].u;
	}

	float GetV(int _n) {
		return uv[_n].v;
	}

	void SetX(int _n, float _x) {
		v[_n].x = _x;
	}

	void SetY(int _n, float _y) {
		v[_n].y = _y;
	}

	void   SetAngle1(int _n, double _angle1) {angle1[_n] = _angle1;};
	void   SetAngle2(int _n, double _angle2) {angle2[_n] = _angle2;};
	double GetAngle1(int _n) {return angle1[_n];};
	double GetAngle2(int _n) {return angle2[_n];};

	void SetTexAnchor(float _f1, float _f2, float _f3, float _f4) {
		tex00x = _f1;
		tex00y = _f2; 
		tex11x = _f3; 
		tex11y = _f4;
	}

	int GetParity(int _p1, int _p2, int _p3);

	void Print(int _p) {
		if (_p >= 0)
			std::cout << GetX(_p) << ":" <<  GetY(_p) << std::endl;
	}

	int GetOverlap(int _p1, int _p2, int _p3, int _p4); //is p1->p2 crossing p3->p4?
	int GetIntersection(int _p1, int _p2, int _p3, int _p4, float *_x, float *_y);

	int   UseANN(void);
	int   GetClosestPoint(float _x, float _y);
	float GetMinDistance(float _x, float _y, float _radius = -1.0f, llQuad *_quad = NULL);
	float GetMinDistance(float _x, float _y, float _radius, llQuadList *_quads);
	float GetMinDistanceGrid(float _x, float _y, float _grid, int _flag = 0);

	float CalculateAngle(float _v1x, float _v1y, float _v1z, float _v2x, float _v2y, float _v2z);

	int GetClosestPointOnLine(float _sx, float _sy, float _sz, float _vx, float _vy, float _vz,
		float _ix, float _iy, float _iz, float *_t, float *_px, float *_py, float *_pz);

	int VectorIntersectsWithTriangle(float _x, float _y, float _z, float _vx, float _vy, float _vz, 
		float _x1, float _y1, float _z1, float _x2, float _y2, float _z2, float _x3, float _y3, float _z3, float *_s, float *_u, float *_v);

	double TriangleMiddleAngle(float _x1, float _y1, float _x2, float _y2, float _x3, float _y3);

	int IsTriangleInsideAngleRange(float _x1, float _y1, float _x2, float _y2, float _x3, float _y3, double _angle1, double _angle2);

};

#endif
