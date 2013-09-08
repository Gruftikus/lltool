#include "../include/llremovetriangulation.h"

llRemoveTriangulation::llRemoveTriangulation() : llTriMod() {
	SetCommandName("RemoveTriangulation");
}

int llRemoveTriangulation::Prepare(void) {
	if (!llTriMod::Prepare()) return 0;
	return 1;
}

int llRemoveTriangulation::RegisterOptions(void) {
	if (!llTriMod::RegisterOptions()) return 0;

	return 1;
}


int llRemoveTriangulation::Exec(void) {
	if (!llTriMod::Exec()) return 0;

	triangles->RemoveTriangles();

	return 1;
}