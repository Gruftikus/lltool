#include "..\include\llsplitatpolygonborder.h"

llSplitAtPolygonBorder::llSplitAtPolygonBorder() : llTriMod() {
	SetCommandName("SplitAtPolygonBorder");
}

int llSplitAtPolygonBorder::Prepare(void) {
	if (!llTriMod::Prepare()) return 0;

	polygon_name = NULL;

	return 1;
}

int llSplitAtPolygonBorder::RegisterOptions(void) {
	if (!llTriMod::RegisterOptions()) return 0;

	RegisterValue("-name", &polygon_name);

	return 1;
}


int llSplitAtPolygonBorder::Init(void) {
	if (!llTriMod::Init()) return 0;

	llPolygonList *polygons = _llMapList()->GetPolygonList(mapname);
	if (!polygons) {
		_llLogger()->WriteNextLine(-LOG_FATAL, "%s: no polygon list in map [%s]", command_name, mapname);
		return 0;
	}

	llPolygon *mypoly = NULL; 
	if (Used("-name")) {
		mypoly = polygons->GetPolygon(polygon_name);
		if (mypoly) {
			for (unsigned int i = 0; i < (unsigned int) mypoly->GetSize()-1;i++) {
				int i1 = mypoly->GetPoint(i);
				int i2 = mypoly->GetPoint(i+1);
				if (i1>=0 && i2>=0) {
					triangles->DivideBetween(points->GetX(i1), points->GetY(i1), points->GetX(i2), points->GetY(i2), map);    	
				}
			}				
		} else {
			_llLogger()->WriteNextLine(-LOG_ERROR,"%s: Polygon not found.", command_name);
		}
	} else {
		for (int i=0;i<polygons->GetSize();i++) {
			mypoly = polygons->GetPolygon(i);
			for (unsigned int i = 0; i < (unsigned int) mypoly->GetSize()-1;i++) {
				int i1 = mypoly->GetPoint(i);
				int i2 = mypoly->GetPoint(i+1);
				if (i1>=0 && i2>=0) {
					triangles->DivideBetween(points->GetX(i1), points->GetY(i1), points->GetX(i2), points->GetY(i2), map);    	
				}
			}	
		}
	}

	_llLogger()->WriteNextLine(LOG_COMMAND,"%s: done", command_name);

	return 1;
}