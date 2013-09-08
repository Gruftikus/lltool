#include "../include/llinactivateallvertices.h"
#include "../include/llmaplist.h"


llInactivateAllVertices::llInactivateAllVertices() : llMapWorker() {
	SetCommandName("InactivateAllVertices");
}

int llInactivateAllVertices::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;
	return 1;
}

int llInactivateAllVertices::Exec(void) {
	if (!llMapWorker::Exec()) return 0;

	if (!points) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: map [%s] has no point list", command_name, mapname);
		return 0;
	}

	int num = 0;

	for (int i=0; i<points->GetN(); i++) {
		if (points->GetActive(i) && 
			points->GetX(i) >= _llUtils()->x00 && points->GetX(i) <= _llUtils()->x11  &&
			points->GetY(i) >= _llUtils()->y00 && points->GetY(i) <= _llUtils()->y11) {
				points->SetInactive(i);
				num++;
		}
	}

	_llLogger()->WriteNextLine(LOG_INFO, "%s: %i vertices inactivated", command_name, num);

	return 1;
}
