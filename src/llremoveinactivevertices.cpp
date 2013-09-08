#include "../include/llremoveinactivevertices.h"

llRemoveInactiveVertices::llRemoveInactiveVertices() : llSet() {
	SetCommandName("RemoveInactiveVertices");
}

int llRemoveInactiveVertices::RegisterOptions(void) {
	if (!llSet::RegisterOptions()) return 0;
	return 1;
}


int llRemoveInactiveVertices::Exec(void) {
	if (!llSet::Exec()) return 0;

	int num = points->GetN() - points->RemoveInactiveVertices();

	_llLogger()->WriteNextLine(LOG_INFO, "%s: %i vertices removed", command_name, num);

	return 1;
}