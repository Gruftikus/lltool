#include "..\include\llsetheight.h"
#include "..\include\llmaplist.h"

llSetHeight::llSetHeight() : llMapWorker() {
	SetCommandName("SetHeight");
}

int llSetHeight::Prepare(void) {
	if (!llMapWorker::Prepare()) return 0;

	usegameunits = 0;

	return 1;
}

int llSetHeight::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;

	RegisterValue("-z",            &zmin, LLWORKER_OBL_OPTION);
	RegisterFlag ("-usegameunits", &usegameunits);

	return 1;
}

int llSetHeight::Exec(void) {
	if (!llMapWorker::Exec()) return 0;

	if (usegameunits) zmin /= map->GetZScale(); //convert to heightmap units
	
	int x1 = (int) map->GetRawX(_llUtils()->x00);
	int x2 = (int) map->GetRawX(_llUtils()->x11);
	int y1 = (int) map->GetRawX(_llUtils()->y00);
	int y2 = (int) map->GetRawX(_llUtils()->y11);

	for (int x=x1; x<=x2; x+=1) {
		for (int y=y1; y<=y2; y+=1) {
			map->SetElementRaw(x, y, zmin);
		}
	}

	return 1;
}
