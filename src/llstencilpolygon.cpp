#include "../include/llstencilpolygon.h"

llStencilPolygon::llStencilPolygon() : llTriMod() {
	SetCommandName("SplitAtPolygonBorder");
}

int llStencilPolygon::Prepare(void) {
	if (!llTriMod::Prepare()) return 0;

	polygon_name = NULL;

	return 1;
}

int llStencilPolygon::RegisterOptions(void) {
	if (!llTriMod::RegisterOptions()) return 0;

	RegisterValue("-name", &polygon_name);

	return 1;
}


int llStencilPolygon::Exec(void) {
	if (!llTriMod::Exec()) return 0;

	llPolygonList *polygons = _llMapList()->GetPolygonList(mapname);
	if (!polygons) {
		_llLogger()->WriteNextLine(-LOG_FATAL, "%s: no polygon list in map [%s]", command_name, mapname);
		return 0;
	}

	llPolygon *mypoly = polygons->GetPolygon(polygon_name);
	int v             = triangles->GetN();
	if (mypoly) {
		//......
		for (unsigned int i = 0; i < (unsigned int) mypoly->GetSize()-1; i++) {
			int i1 = mypoly->GetPoint(i);
			int i2 = mypoly->GetPoint(i+1);
			if (i1>=0 && i2>=0) {
				triangles->DivideBetween(points->GetX(i1), points->GetY(i1), points->GetX(i2), points->GetY(i2), map);    	
			}
		}
		//........as above
		v = triangles->GetN();
		for (int i=0;i<v;i++) {

			int o1 = triangles->GetPoint1(i);
			int o2 = triangles->GetPoint2(i);
			int o3 = triangles->GetPoint3(i);

			//all 3 points and the center of gravity inside?

			int num = /*mypoly->IsPointInsidePolygon(points->GetX(o1),points->GetY(o1)) +
					  mypoly->IsPointInsidePolygon(points->GetX(o2),points->GetY(o2)) +
					  mypoly->IsPointInsidePolygon(points->GetX(o3),points->GetY(o3)) +*/
					  mypoly->IsPointInsidePolygon((points->GetX(o1)+points->GetX(o2)+points->GetX(o3))/3.f,
					  (points->GetY(o1)+points->GetY(o2)+points->GetY(o3))/3.f);

			if (num == 1) {
				triangles->RemoveTriangle(i);
				i--;
				v--;
			}

		}
	} else {
		_llLogger()->WriteNextLine(-LOG_ERROR,"%s: Polygon not found.", command_name);
		return 0;
	}

	_llLogger()->WriteNextLine(-LOG_COMMAND,"%s: done", command_name);

	return 1;
}