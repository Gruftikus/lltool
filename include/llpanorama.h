#ifndef _PLLPANORAMA_H_
#define _PLLPANORAMA_H_

#include "../include/llset.h"

class llPanorama : public llSet {

private:

	float gx, gy, keepout;

public:

	llPanorama();

	virtual llWorker * Clone() {
		return new llPanorama(*this);
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
