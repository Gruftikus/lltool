#ifndef _PLLEXIT_H_
#define _PLLEXIT_H_

#include "llworker.h"

class llExit : public llWorker {

protected:

public:

	llExit();

	virtual llWorker * Clone() {
		return new llExit(*this);
	}

	virtual int RegisterOptions(void);
	virtual int Init(void);

};

#endif
