#include "..\include\llremoveinactivetriangles.h"
#include <string.h>
#include <stdio.h>

//constructor
llRemoveInactiveTriangles::llRemoveInactiveTriangles() : llTriMod() {

	SetCommandName("RemoveInactiveTriangles");

}

int llRemoveInactiveTriangles::RegisterOptions(void) {
	if (!llTriMod::RegisterOptions()) return 0;
	return 1;
}


int llRemoveInactiveTriangles::Init(void) {
	if (!llTriMod::Init()) return 0;

	int v = triangles->GetN();
	int num=0;
	for (int i=0;i<v;i++) {

		int o1 = triangles->GetPoint1(i);
		int o2 = triangles->GetPoint2(i);
		int o3 = triangles->GetPoint3(i);

		if (!points->GetActive(o1) && !points->GetActive(o2) && !points->GetActive(o3)) {
			triangles->RemoveTriangle(i);
			i--;v--;num++;
		}
	}
	_llLogger()->WriteNextLine(LOG_INFO,"%s: %i triangles removed", command_name, num);

	return 1;
}