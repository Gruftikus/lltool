#include "../include/llactivatevisiblevertices.h"

llActivateVisibleVertices::llActivateVisibleVertices() : llTriMod() {
	SetCommandName("ActivateVisibleVertices");
}

int llActivateVisibleVertices::RegisterOptions(void) {
	if (!llTriMod::RegisterOptions()) return 0;

	RegisterValue("-x",      &x);
	RegisterValue("-y",      &y);
	RegisterValue("-z",      &z);
	RegisterValue("-radius", &radius);

	return 1;
}

int llActivateVisibleVertices::Prepare(void) {
	if (!llTriMod::Prepare()) return 0;

	x = y = z = radius = 0;

	return 1;
}

int llActivateVisibleVertices::Exec(void) {
	if (!llTriMod::Exec()) return 0;

	int n_rounds = 1;
	if (!Used("-radius")) {
		n_rounds = 1 + 6;	
	} 

	int v = triangles->GetN();
	unsigned int total_steps = n_rounds * v;
	unsigned int last_step = 0;
	for (int round=0; round < n_rounds; round++) {
		for (int i=0; i<v; i++) {

			int o1 = triangles->GetPoint1(i);
			int o2 = triangles->GetPoint2(i);
			int o3 = triangles->GetPoint3(i);

			unsigned int current_step = (round * v) + i;
			if ( ((current_step * 100) / total_steps) != last_step) {
				last_step = (current_step * 100) / total_steps;
				_llLogger()->WriteNextLine(-LOG_INFO, "%i percent complete", last_step);
			}

			for (int j=0; j<points->GetN(); j++) {
				if (j!=o1 && j!=o2 && j!=o3) {
					float vx = points->GetX(j) - x;
					float vy = points->GetY(j) - y;
					float vz = points->GetZ(j) - z;
					float s, u, v;
					float sx = x;
					float sy = y;
					float sz = z;

					if (round == 1) {
						sx += radius;
					} else if (round == 2) {
						sx -= radius;
					} else if (round == 3) {
						sy += radius;
					} else if (round == 4) {
						sy -= radius;
					} else if (round == 5) {
						sz += radius;
					} else if (round == 6) {
						sz -= radius;
					} 

					if (sz < map->GetZ(sx, sy)) {
						sz = map->GetZ(sx, sy);
					}

					if (!points->GetActive(j)) {
						//skip active points, they don't need to be checked again
						//because they are visible from "somewhere"
						if (points->VectorIntersectsWithTriangle(sx, sy, sz,
							vx, vy, vz, points->GetX(o1), points->GetY(o1), points->GetZ(o1),
							points->GetX(o2), points->GetY(o2), points->GetZ(o2),
							points->GetX(o3), points->GetY(o3), points->GetZ(o3), &s, &u, &v)) {
								points->SetTmpInactive(j);
						}
					} 
				}
			}
		}
		points->ClearTmpInactive();
	} //rounds

	return 1;
}
