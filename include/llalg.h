#ifndef _PLLALG_H_
#define _PLLALG_H_

#include "../include/llworker.h"
#include "../include/llmap.h"

class llAlg : public llWorker {

 protected:

	char  *alg_list;
	char  *std_alg_list;	
	char  *mapname;
	char  *std_mapname;
	llMap *map;

    float  x00, y00, x11, y11; //focus

    double loc_ceiling;

	double add, multiply;

 public:

    
    //constructor
    llAlg(char *_alg_list, char *_map);

    virtual double GetCeiling(double *_ceiling=NULL) = 0; 
	virtual double GetValue(float _x, float _y, double *_value=NULL) = 0; 

	virtual llWorker * Clone() {
		std::cout << "I should never be here...." << std::endl;
		return new llWorker(*this);
	}

	virtual int Prepare(void);
	virtual int RegisterOptions(void);
	virtual int Exec(void);

};

#endif
