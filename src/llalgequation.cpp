#include "../include/llalgequation.h"

llAlgEquation::llAlgEquation(char*_alg_list, char *_map) : llAlg(_alg_list, _map) {
	SetCommandName("AlgEquation");
}

int llAlgEquation::Prepare(void) {
	if (!llAlg::Prepare()) return 0;

	parser = new MTParser();

	parser->defineVar(_T("x"), &x);
	parser->defineVar(_T("y"), &y);
	parser->defineVar(_T("z"), &z);

	blue = red = green = alpha = 0;
	zmin = -999999999.f;
	zmax =  999999999.f;

	return 1;
}

int llAlgEquation::RegisterOptions(void) {
	if (!llAlg::RegisterOptions()) return 0;

	RegisterValue("-equation", &parser_string, LLWORKER_OBL_OPTION);

	RegisterValue("-zmin", &zmin);
	RegisterValue("-zmax", &zmax);

	RegisterFlag("-blue",   &blue);
	RegisterFlag("-red",    &red);
	RegisterFlag("-green",  &green);
	RegisterFlag("-alpha",  &alpha);

	return 1;
}

double llAlgEquation::GetCeiling(double *_ceiling) {

	double loc_ceiling=1.;

	if (_ceiling) {
		if (add)
			*_ceiling += loc_ceiling*add;
		if (multiply)
			*_ceiling *= loc_ceiling*multiply;
		return *_ceiling;
	} else {
		return loc_ceiling*add*multiply;
	}
}

double llAlgEquation::GetValue(float _x, float _y, double *_value) {

	x = _x;
	y = _y;

	if (map) { //works because std_map is NULL
		if (!blue && !red && !green && !alpha) {
			float c = map->GetZ(_x, _y);
			if (c < zmin) c = zmin;
			if (c > zmax) c = zmax;
			z = c;
		} else {
			unsigned char byte1;
			unsigned char byte2;
			unsigned char byte3;
			unsigned char byte4;
			map->GetTupel(map->GetRawX(_x), map->GetRawY(_y), &byte1, &byte2, &byte3, &byte4);
			float c = 1;
			if (blue)  c *= float(byte1)/255.f;
			if (green) c *= float(byte2)/255.f;
			if (red)   c *= float(byte3)/255.f;
			if (alpha) c *= float(byte4)/255.f;
			if (c < zmin) c = zmin;
			if (c > zmax) c = zmax;
			z = c;
		}	
	}

	double loc_value = parser->evaluate();

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

int llAlgEquation::Exec(void) {

	if (!llAlg::Exec()) return 0;

	parser->compile(_T(parser_string));

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
