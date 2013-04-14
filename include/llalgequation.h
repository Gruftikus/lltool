#ifndef _PLLALGEQUATION_H_
#define _PLLALGEQUATION_H_

#include "../include/llmap.h"
#include "../include/llalglist.h"
#include "../externals/mtparser/MTParserLib/MTParser.h"
#include "../externals/mtparser/MTParserLib/MTParserLocalizer.h"

class llAlgEquation : public llAlg {

private:

	MTParser *parser;
	char     *parser_string;
	MTDOUBLE  x, y;

public:

	//constructor
	llAlgEquation(char *_alg_list, char *_map);

	double GetCeiling(double *ceiling = NULL); 
	double GetValue(float x, float y, double *value = NULL); 

	virtual llWorker * Clone() {
		return new llAlgEquation(*this);
	}

	int    Prepare(void);
	int    RegisterOptions(void);
	int    Exec(void);
};

#endif
