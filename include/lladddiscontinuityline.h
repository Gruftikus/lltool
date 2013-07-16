#ifndef _PLLADDDISCONTINUITYLINE_H_
#define _PLLADDDISCONTINUITYLINE_H_

#include "../include/llset.h"

class llAddDiscontinuityLine : public llSet {

private:

	float x1, y1, x2, y2;

public:

	llAddDiscontinuityLine();

	virtual llWorker * Clone() {
		return new llAddDiscontinuityLine(*this);
	}

	int RegisterOptions(void);
	int Exec(void);

};

#endif
