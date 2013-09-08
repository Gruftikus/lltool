#include "../include/llactivatefractionofvertices.h"

llActivateFractionOfVertices::llActivateFractionOfVertices() : llSet() {
	SetCommandName("ActivateFractionOfVertices");
}

int llActivateFractionOfVertices::RegisterOptions(void) {
	if (!llSet::RegisterOptions()) return 0;

	RegisterValue("-fraction", &n, LLWORKER_OBL_OPTION);

	return 1;
}

int llActivateFractionOfVertices::Prepare(void) {
	if (!llSet::Prepare()) return 0;

	return 1;
}

int llActivateFractionOfVertices::Exec(void) {
	if (!llSet::Exec()) return 0;

	if (n <= 0 || n >= 1) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: fraction must be >0 and <1", command_name);
		return 0;
	}

	int free = 0;

	for (int i=0; i<points->GetN(); i++) {
		if (!points->GetActive(i) && 
			points->GetX(i) >= _llUtils()->x00 && points->GetX(i) <= _llUtils()->x11  &&
			points->GetY(i) >= _llUtils()->y00 && points->GetY(i) <= _llUtils()->y11)
			free++;
	}
	//std::cout << free << std::endl;

	int to_set = int(float(free) * n);
	//std::cout << to_set << std::endl;

	int running = 0;
	for (int i=0; i<to_set; i++) {
		int num = int(double(rand())/double(RAND_MAX) * double(free));
		int got_it = 0;
		//std::cout << num << std::endl;
		while (!got_it) {
			if (!points->GetActive(running) && 
				points->GetX(running) >= _llUtils()->x00 && points->GetX(running) <= _llUtils()->x11  &&
				points->GetY(running) >= _llUtils()->y00 && points->GetY(running) <= _llUtils()->y11) 
				num--;
			if (num) 
				running++;
			else
				got_it = 1;
			if (running == points->GetN()) running = 0;
		}

		points->SetActive(running);
		free--;
		
	}

	_llLogger()->WriteNextLine(LOG_INFO, "%s: %i vertices activated", command_name, to_set);
	return 1;
}
