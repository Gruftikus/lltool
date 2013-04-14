#include "../include/llalgequation.h"

llAlgEquation::llAlgEquation(char*_alg_list, char *_map) : llAlg(_alg_list, _map) {
	SetCommandName("AlgEquation");
}

int llAlgEquation::Prepare(void) {
	if (!llAlg::Prepare()) return 0;

	parser = new MTParser();

	parser->defineVar(_T("x"), &x);
	parser->defineVar(_T("y"), &y);

	return 1;
}

int llAlgEquation::RegisterOptions(void) {
	if (!llAlg::RegisterOptions()) return 0;

	RegisterValue("-equation", &parser_string, LLWORKER_OBL_OPTION);

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
