#ifndef _PLLADDDISCONTINUITYGRID_H_
#define _PLLADDDISCONTINUITYGRID_H_

#include "../include/llset.h"

class llAddDiscontinuityGrid : public llSet {

private:

	float gridx, gridy;

public:

	llAddDiscontinuityGrid();

	virtual llWorker * Clone() {
		return new llAddDiscontinuityGrid(*this);
	}

	int RegisterOptions(void);
	int Exec(void);

};

#endif
