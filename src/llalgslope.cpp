#include "../include/llalgslope.h"

llAlgSlope::llAlgSlope(char *_alg_list, char *_map) : llAlg(_alg_list, _map) {
	loc_ceiling = 0;
	SetCommandName("AlgLinear");
}

int llAlgSlope::Prepare(void) {
	if (!llAlg::Prepare()) return 0;

	lowest           = -8000;
	highest          =  8000;
	value_at_lowest  =  0.2f;
	value_at_highest =  1.0f;

	return 1;
}

int llAlgSlope::RegisterOptions(void) {
	if (!llAlg::RegisterOptions()) return 0;

	RegisterValue("-zmax",     &highest);
	RegisterValue("-zmin",     &lowest);
	RegisterValue("-zminval",  &value_at_lowest);
	RegisterValue("-zmaxval",  &value_at_highest);	

	return 1;
}

double llAlgSlope::GetCeiling(double *_ceiling) {

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

double llAlgSlope::GetValue(float _x, float _y, double *_value) {

	double loc_value = 0;

	float z = map->GetZ(_x, _y);

	if (z < lowest) 
		loc_value = value_at_lowest;
	else if (z > highest) {
		loc_value = value_at_highest;
	} else {
		loc_value= value_at_lowest + 
			((z - lowest)/(highest - lowest)) * (value_at_highest - value_at_lowest);
	}

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

int llAlgSlope::Exec(void) {
	if (!llAlg::Exec()) return 0;

	if (alg_list) {
		llAlgCollection *algs = _llAlgList()->GetAlgCollection(alg_list);
		if (!algs) {
			_llLogger()->WriteNextLine(-LOG_FATAL, "%s: alg collection [%s] not found", command_name, alg_list);
			return 0;
		}
		algs->AddAlg(this);
	}

	return 1;
}
