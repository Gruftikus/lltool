#include "../include/llsetflag.h"

llSetFlag::llSetFlag() : llWorker() {
	SetCommandName("SetFlag");
}

int llSetFlag::Prepare(void) {
	if (!llWorker::Prepare()) return 0;

	value   = NULL;
	hidden  = unselect = zerosuppression = 0;
	replace = with     = NULL;
	num     = -1;

	return 1;
}

int llSetFlag::RegisterOptions(void) {
	if (!llWorker::RegisterOptions()) return 0;

	RegisterValue("-name",     &name, LLWORKER_OBL_OPTION);
	RegisterValue("-value",    &value);

	RegisterValue("-replace",  &replace);
	RegisterValue("-with",     &with);
	RegisterValue("-num",      &num);

	RegisterFlag ("-hidden",   &hidden);
	RegisterFlag ("-unselect", &unselect);

	RegisterFlag ("-zerosuppression", &zerosuppression);

	return 1;
}

int llSetFlag::Exec(void) {
	llWorker::Exec();
	
	if (value) {
		if (replace && with) value = _llUtils()->Replace(value, replace, with, num);
		_llUtils()->SetValue(name, value);
		if (hidden) _llUtils()->SetHidden(name);
	} else {
		_llUtils()->AddFlag(name);
		if (hidden) _llUtils()->SetHidden(name);
	} 
	if (unselect) _llUtils()->DisableFlag(name);
	if (zerosuppression && !*(_llUtils()->GetValueF(name))) _llUtils()->DisableFlag(name);

	return 1;
}
