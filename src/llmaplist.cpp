//a list of algorithms

#include "../include/llmaplist.h"
#include <string.h>
#include <stdio.h>
#include <math.h>


llMapList& _fllMapList() {
    static llMapList* ans = new llMapList();
    return *ans;
}

llMapList * _llMapList() {
    return &_fllMapList();
}

llMapList::llMapList() {
	map_list.resize(0);
	map_name.resize(0);
}

int llMapList::AddMap(const char *_name, llMap *_map, llPointList *_points, llTriangleList *_triangles, llPolygonList *_polygons, llLineList *_lines) {
	int size = GetSize();
	for (int i=0; i<size; i++) {
		if (_stricmp(_name, map_name[i]) == 0) {
			_llLogger()->WriteNextLine(-LOG_ERROR,"llMapList::AddMap: map %s already existing", _name);
			return 0;
		}
	}
	map_list.resize(size + 1);
	map_list[size]      = _map;
	map_name.resize(size + 1);
	map_name[size]      = _name;
	point_list.resize(size + 1);
	point_list[size]    = _points;
	line_list.resize(size + 1);
	line_list[size]     = _lines;
	triangle_list.resize(size + 1);
	triangle_list[size] = _triangles;
	polygon_list.resize(size + 1);
	polygon_list[size]  = _polygons;
	return 1;
}

int llMapList::AddMap(const char *_name, llMap *_map, const char *_oldmap) {
	int size = GetSize();
	for (int i=0; i<size; i++) {
		if (_stricmp(_name, map_name[i]) == 0) {
			_llLogger()->WriteNextLine(-LOG_ERROR,"llMapList::AddMap: map %s already existing", _name);
			return 0;
		}
	}
	int num = GetMapNum(_oldmap);
	if (num<0) {
		_llLogger()->WriteNextLine(-LOG_ERROR,"llMapList::AddMap: source map %s not found", _name);
		return 0;
	}
	map_list.resize(size + 1);
	map_list[size]      = _map;
	map_name.resize(size + 1);
	map_name[size]      = _name;
	point_list.resize(size + 1);
	point_list[size]    = point_list[num];
	line_list.resize(size + 1);
	line_list[size]     = line_list[num];
	triangle_list.resize(size + 1);
	triangle_list[size] = triangle_list[num];
	polygon_list.resize(size + 1);
	polygon_list[size]  = polygon_list[num];

	return 1;
}

void llMapList::ExchangeMap(char *_name, llMap *_map) {
	int size = GetSize();
	for (int i=0; i<size; i++) {
		if (_stricmp(_name, map_name[i]) == 0) {
			map_list[i] = _map;
			if (polygon_list[i]) polygon_list[i]->SetMap(_map);
			return;
		}
	}
	_llLogger()->WriteNextLine(-LOG_ERROR,"llMapList::ExchangeMap: map %s not found", _name);
}


llMap *llMapList::GetMap(const char* _name) {
	//cout << "GetMap " << _name  << endl;
	for (int i=0; i<GetSize(); i++) {
		if (_stricmp(_name, map_name[i]) == 0)
			return map_list[i];
	}
	return NULL;
}

int llMapList::DeleteMap(char *_name) {
	//deletes the map extry, and deletes also the object(s)
	//if not shared with other entries.

	int num = GetMapNum(_name);
	if (num<0) {
		_llLogger()->WriteNextLine(-LOG_ERROR,"llMapList::DeleteMap: map %s not found", _name);
		return 0;
	}

	llMap          *map       = map_list[num];
	llPointList    *points    = point_list[num];
	llTriangleList *triangles = triangle_list[num];
	llLineList     *lines     = line_list[num];
	llPolygonList  *poly      = polygon_list[num];

	for (int i=0; i<GetSize(); i++) {
		if (i != num) {
			if (map == map_list[i])            map       = NULL;
			if (points == point_list[i])       points    = NULL;
			if (lines  == line_list[i])        lines     = NULL;
			if (triangles == triangle_list[i]) triangles = NULL;
			if (poly == polygon_list[i])       poly      = NULL;
		}
	}

	if (!map) delete map;
	if (!points) delete points;
	if (!lines) delete lines;
	if (!triangles) delete triangles;
	if (!poly) delete poly;

	for (int i=num; i<GetSize()-1; i++) {
		map_name[i]      = map_name[i+1];
		map_list[i]      = map_list[i+1];
		point_list[i]    = point_list[i+1];
		line_list[i]     = line_list[i+1];
		triangle_list[i] = triangle_list[i+1];
		polygon_list[i]  = polygon_list[i+1];
	}

	int size = GetSize();

	map_list.resize(size - 1);
	map_name.resize(size - 1);
	point_list.resize(size - 1);
	line_list.resize(size - 1);
	triangle_list.resize(size - 1);
	polygon_list.resize(size - 1);

	return 1;

}

int llMapList::GetNumHeights(char *_mapname, float _x, float _y, double *_angles, float *_z, int _size) {
	//returns the number of possible height of the map _mapname
	//at position _x, _y

	llLineList *lines = GetLineList(_mapname);
	if (!lines) return 1; //no lines, no problem...

	llPointList *points = GetPointList(_mapname);
	if (!points) return 1;

	llMap *map = GetMap(_mapname);

	int angles_counter = 0;

	float scansize = 1.f;

	for (int i=0; i<lines->GetN(); i++) {
		float t, px, py, pz;

		points->GetClosestPointOnLine(lines->GetLine(i)->GetX1(), lines->GetLine(i)->GetY1(), 0,
			lines->GetLine(i)->GetX2() - lines->GetLine(i)->GetX1(), lines->GetLine(i)->GetY2() - lines->GetLine(i)->GetY1(), 0,
			_x, _y, 0, &t, &px, &py, &pz);

		float distance = sqrt((px - _x)*(px - _x) + (py - _y)*(py - _y));

		if (distance <= scansize && t >= 0 && t <= 1) {
			double angle = 0.;
			if (t < 0.5)
				angle = atan2((lines->GetLine(i)->GetY2() - lines->GetLine(i)->GetY1()), 
				(lines->GetLine(i)->GetX2() - lines->GetLine(i)->GetX1()));
			else
				angle = atan2((lines->GetLine(i)->GetY1() - lines->GetLine(i)->GetY2()), 
				(lines->GetLine(i)->GetX1() - lines->GetLine(i)->GetX2()));
			if (angles_counter < _size-1) {
				_angles[angles_counter++] = angle;

				if (sqrt((px - lines->GetLine(i)->GetX1())*(px - lines->GetLine(i)->GetX1()) + (py - lines->GetLine(i)->GetY1())*(py - lines->GetLine(i)->GetY1())) > scansize &&
					sqrt((px - lines->GetLine(i)->GetX2())*(px - lines->GetLine(i)->GetX2()) + (py - lines->GetLine(i)->GetY2())*(py - lines->GetLine(i)->GetY2())) > scansize) {
						//in the middle of the line
						_angles[angles_counter++] = angle + M_PI;
				}

			} else {
				_llLogger()->WriteNextLine(-LOG_ERROR, "too many discontinuities in map [%s] at (%f, %f)", _mapname, _x, _y);
			}
		}
	}

	for (int i=0; i<angles_counter; i++) {
		if (_angles[i] < 0)      _angles[i] += 2*M_PI;
		if (_angles[i] > 2*M_PI) _angles[i] -= 2*M_PI;
		if (_angles[i] < 0.001)  _angles[i]  = 0.;
	}

	//bubble sort
	for (int i=0; i<angles_counter; i++) {
		for (int j=i-1; j>=0; j--) {
			if (_angles[j] > _angles[j+1]) {
				double x     = _angles[j];
				_angles[j]   = _angles[j+1];
				_angles[j+1] = x;
			}
		}
	}

	//for (int i=0; i<angles_counter; i++) std::cout << _angles[i] << std::endl;

	//calculate section angles
	float *secangles = new float[_size];
	//first section angle must take the last one into account
	secangles[0] = (_angles[0] + (_angles[angles_counter-1] - 2*M_PI)) / 2.;
	if (secangles[0] < 0) secangles[0] += 2*M_PI;

	//std::cout << "sec0:" << secangles[0] << std::endl;

	for (int i=1; i<angles_counter; i++) {
		secangles[i] = (_angles[i] + _angles[i-1]) / 2.;
		//std::cout << "sec:" << secangles[i] << std::endl;
	}

	//find pixel
	float dx = map->GetWidthXPerRaw()/1.9f;
	float dy = map->GetWidthYPerRaw()/1.9f;

	// std::cout << "****** " << _x << ":" << _y << std::endl;

	for (int i=0; i<angles_counter; i++) {
		//float x = _x + 0.1*map->GetWidthXPerRaw();
		//float y = _y + 0.1*map->GetWidthYPerRaw();
		float x = _x;
		float y = _y;

		if (secangles[i] < M_PI/8.)          {x += dx;}
		else if (secangles[i] < M_PI*3./8.)  {x += dx; y += dy;}
		else if (secangles[i] < M_PI*5./8.)  {y += dy;}
		else if (secangles[i] < M_PI*7./8.)  {x -= dx; y += dy;}
		else if (secangles[i] < M_PI*9./8.)  {x -= dx;}
		else if (secangles[i] < M_PI*11./8.) {x -= dx; y -= dy;}
		else if (secangles[i] < M_PI*13./8.) {y -= dy;}
		else if (secangles[i] < M_PI*15./8.) {x += dx; y -= dy;}
		else                                 {x += dx;}

		_z[i] = map->GetZ(x, y);

#if 0
		//if (angles_counter > 2) {
			std::cout << "angles:" << _angles[i] << ":"<< secangles[i] << std::endl;
			std::cout << x << ":" << y << std::endl;
		//}
#endif
	}

	delete secangles;
	return angles_counter;

}