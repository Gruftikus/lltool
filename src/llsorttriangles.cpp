#include "../include/llsorttriangles.h"

llSortTriangles::llSortTriangles() : llTriMod() {
	SetCommandName("SortTriangles");
}

int llSortTriangles::Prepare(void) {
	if (!llTriMod::Prepare()) return 0;

	yx = 0;

	return 1;
}

int llSortTriangles::RegisterOptions(void) {
	if (!llTriMod::RegisterOptions()) return 0;

	RegisterValue("-x", &gridx, LLWORKER_OBL_OPTION);
	RegisterValue("-y", &gridy, LLWORKER_OBL_OPTION);

	RegisterFlag ("-yx", &yx);

	return 1;
}


int llSortTriangles::Exec(void) {
	if (!llTriMod::Exec()) return 0;

	triangles->SetRange(_llUtils()->x00, _llUtils()->x11, _llUtils()->y00, _llUtils()->y11);
	triangles->SortTrianglesQuick(gridx, gridy, yx);

	return 1;
}