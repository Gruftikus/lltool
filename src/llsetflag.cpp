#include "..\include\llsetflag.h"

llSetFlag::llSetFlag() : llWorker() {
	SetCommandName("SetFlag");
}

int llSetFlag::Prepare(void) {
	if (!llWorker::Prepare()) return 0;

	value = NULL;
	hidden = unselect = 0;

	return 1;
}

int llSetFlag::RegisterOptions(void) {
	if (!llWorker::RegisterOptions()) return 0;

	RegisterValue("-name",     &name, LLWORKER_OBL_OPTION);
	RegisterValue("-value",    &value);
	RegisterFlag ("-hidden",   &hidden);
	RegisterFlag ("-unselect", &unselect);

	return 1;
}

int llSetFlag::Exec(void) {
	llWorker::Exec();

	if (value) {
		_llUtils()->SetValue(name, value);
		if (hidden) _llUtils()->SetHidden(name);
	} else {
		_llUtils()->AddFlag(name);
		if (hidden) _llUtils()->SetHidden(name);
	} 
	if (unselect) _llUtils()->DisableFlag(name);
	return 1;
}
