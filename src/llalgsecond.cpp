
#include "../include/llalgsecond.h"
#include "../include/llmaplist.h"

llAlgSecond::llAlgSecond(char *_alg_list, char *_map) : llAlg(_alg_list, _map) {
	loc_ceiling = 0;
	SetCommandName("AlgSecondOrder");
}

int llAlgSecond::RegisterOptions(void) {
	if (!llAlg::RegisterOptions()) return 0;
	return 1;
}

double llAlgSecond::GetCeiling(double *_ceiling) {
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

double llAlgSecond::GetValue(float _x, float _y, double *_value) {

	if (!mapx2 || !mapy2) return 0.;

	double loc_value = 0;
	unsigned int xx = map->GetRawX(_x);
	unsigned int yy = map->GetRawY(_y);

	if (_x>=x00 && _x<=x11 && _y>=y00 && _y<=y11) {
		loc_value =
			( fabs(mapx2->GetZ(xx, yy)) 
			+ fabs(mapy2->GetZ(xx, yy)) );
	}

	//std::cout << loc_value << ":" << loc_ceiling << std::endl;

	if (loc_value > loc_ceiling) 
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

	return loc_value;
}

int llAlgSecond::Exec(void) {
	if (!llAlg::Exec()) return 0;

	if (alg_list) {
		llAlgCollection *algs = _llAlgList()->GetAlgCollection(alg_list);
		if (!algs) {
			_llLogger()->WriteNextLine(-LOG_FATAL, "%s: alg collection [%s] not found", command_name, alg_list);
			return 0;
		}
		algs->AddAlg(this);
	}

	char * namex2 = new char[strlen(mapname) + 5];
	sprintf_s(namex2, strlen(mapname)+5, "%s_d2x", mapname);
	mapx2 = _llMapList()->GetMap(namex2);
	if (!mapx2) {
		_llLogger()->WriteNextLine(-LOG_WARNING,"%s: derivative map %s not existing", command_name, namex2);
		return 0;
	}
	char * namey2 = new char[strlen(mapname) + 5];
	sprintf_s(namey2, strlen(mapname)+5, "%s_d2y", mapname);
	mapy2 = _llMapList()->GetMap(namey2);
	if (!mapy2) {
		_llLogger()->WriteNextLine(-LOG_WARNING,"%s: derivative map %s not existing", command_name, namey2);
		return 0;
	}

	return 1;
}
