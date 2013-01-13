
#include "..\include\llalgfirst.h"
#include "..\include\llmaplist.h"

llAlgFirst::llAlgFirst(char *_alg_list, char *_map) : llAlg(_alg_list, _map) {

	loc_ceiling = 0;

	SetCommandName("AlgFirstOrder");
}

int llAlgFirst::RegisterOptions(void) {
	if (!llAlg::RegisterOptions()) return 0;
	return 1;
}

double llAlgFirst::GetCeiling(double *_ceiling) {
	if (_ceiling) {
		if (add)
			*_ceiling += loc_ceiling*add;
		if (multiply)
			*_ceiling *= loc_ceiling*multiply;
		return *_ceiling;
	} else {
		return loc_ceiling*multiply + add*loc_ceiling;
	}
}

double llAlgFirst::GetValue(float _x, float _y, double *_value) {

	if (!mapx1 || !mapy1) return 0.;

	double loc_value = 0;
	unsigned int xx = map->GetRawX(_x);
	unsigned int yy = map->GetRawY(_y);

	if (_x>=x00 && _x<=x11 && _y>=y00 && _y<=y11) {
		loc_value =
			( fabs(mapx1->GetZ(xx, yy)) 
			+ fabs(mapy1->GetZ(xx, yy)) );
	}

	if (loc_value > loc_ceiling && loc_value < 2.0f) //maxslope is 200%
		loc_ceiling = loc_value;

	if (_value) {
		if (add)
			*_value += loc_value*add;
		if (multiply)
			*_value *= loc_value*multiply;
		return *_value;
	} else {
		return loc_value*multiply + add*loc_value;
	}
}

int llAlgFirst::Init(void) {

	if (!llAlg::Init()) return 0;

	if (alg_list) {
		llAlgCollection *algs = _llAlgList()->GetAlgCollection(alg_list);
		if (!algs) {
			_llLogger()->WriteNextLine(-LOG_FATAL, "%s: alg collection [%s] not found", command_name, alg_list);
			return 0;
		}
		algs->AddAlg(this);
	}

	char * namex1 = new char[strlen(mapname) + 5];
	sprintf_s(namex1, strlen(mapname)+5, "%s_d1x", mapname);
	mapx1 = _llMapList()->GetMap(namex1);
	if (!mapx1) {
		_llLogger()->WriteNextLine(-LOG_WARNING,"%s: derivative map %s not existing", command_name, namex1);
		return 0;
	}
	char * namey1 = new char[strlen(mapname) + 5];
	sprintf_s(namey1, strlen(mapname)+5, "%s_d1y", mapname);
	mapy1 = _llMapList()->GetMap(namey1);
	if (!mapy1) {
		_llLogger()->WriteNextLine(-LOG_WARNING,"%s: derivative map %s not existing", command_name, namey1);
		return 0;
	}


	return 1;
}