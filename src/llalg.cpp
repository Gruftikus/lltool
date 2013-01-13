//base class for all algorithms

#include "..\include\llalg.h"
#include "..\include\llmaplist.h"

llAlg::llAlg(char *_alg_list, char *_map) : llWorker() {
	std_mapname  = _map;
	std_alg_list = _alg_list;
}

int llAlg::Prepare(void) {
	if (!llWorker::Prepare()) return 0;

	alg_list = std_alg_list;
	mapname  = std_mapname;

	return 1;
}

int llAlg::RegisterOptions(void) {
	if (!llWorker::RegisterOptions()) return 0;

	RegisterValue("-map",      &mapname);
	RegisterValue("-alg",      &alg_list);
	RegisterValue("-multiply", &multiply);
	RegisterValue("-add",      &add);

	return 1;
}

int llAlg::Init(void) {
	llWorker::Init();

	//get the corresponding map from the global map container
	if (mapname) {
		map = _llMapList()->GetMap(mapname);
		if (!map) {
			_llLogger()->WriteNextLine(-LOG_FATAL, "%s: map [%s] not found", command_name, mapname);
			return 0;
		}
	}

	if (!Used("-add") && !Used("-multiply")) {
		_llLogger()->WriteNextLine(-LOG_WARNING,"%s: no add or multiply factor specified (assuming -multiply=1)", GetCommandName());
		multiply = 1.;
		add = 0.;
	} else { 
		if (!Used("-add")) {
			add = 0.;
		} 
		if (!Used("-multiply")) {
			multiply = 0.;
		}
	}

	//get the focus from the global utility container
	x00 = _llUtils()->x00;
	y00 = _llUtils()->y00;
	x11 = _llUtils()->x11;
	y11 = _llUtils()->y11;

	return 1;
}
