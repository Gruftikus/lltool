#include "../include/llsetheight.h"
#include "../include/llmaplist.h"
#include "../include/llalglist.h"

llSetHeight::llSetHeight() : llMapWorker() {
	SetCommandName("SetHeight");
}

int llSetHeight::Prepare(void) {
	if (!llMapWorker::Prepare()) return 0;

	usegameunits = 0;
	alg_list     = NULL;
	min = max    = 0;

	return 1;
}

int llSetHeight::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;

	RegisterValue("-z",            &zmin, LLWORKER_OBL_OPTION);
	RegisterValue("-alg",          &alg_list);

	RegisterFlag ("-usegameunits", &usegameunits);
	RegisterFlag ("-min",          &min);
	RegisterFlag ("-max",          &max);

	return 1;
}

int llSetHeight::Exec(void) {
	if (!llMapWorker::Exec()) return 0;

	llAlgCollection *algs = NULL;
	if (Used("-alg")) {
		algs = _llAlgList()->GetAlgCollection(alg_list);
		if (!algs) {
			_llLogger()->WriteNextLine(-LOG_FATAL, "%s: alg collection '%s' not found", command_name, alg_list);
			return 0;
		}
	}

	if (usegameunits) zmin /= map->GetZScale(); //convert to heightmap units
	
	int x1 = (int) map->GetRawX(_llUtils()->x00);
	int x2 = (int) map->GetRawX(_llUtils()->x11);
	int y1 = (int) map->GetRawY(_llUtils()->y00);
	int y2 = (int) map->GetRawY(_llUtils()->y11);

	for (int x=x1; x<=x2; x+=1) {
		for (int y=y1; y<=y2; y+=1) {
			double alg = 0.0;
			if (algs) {
				alg = algs->GetValue(map->GetCoordX(x), map->GetCoordY(y));
			}
			if (min && map->GetElementRaw(x, y) < zmin)
				map->SetElementRaw(x, y, zmin + alg);
			else if (max && map->GetElementRaw(x, y) > zmin)
				map->SetElementRaw(x, y, zmin + alg);
			else if (!min && !max)
				map->SetElementRaw(x, y, zmin + alg);
		}
	}

	return 1;
}
