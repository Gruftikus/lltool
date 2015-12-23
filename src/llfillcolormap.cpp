#include "../include/llfillcolormap.h"
#include "../include/llmaplist.h"
#include "../include/llalglist.h"

//constructor
llFillColorMap::llFillColorMap() : llMapWorker() {
	SetCommandName("FillColorMap");
}

int llFillColorMap::Prepare(void) {
	if (!llMapWorker::Prepare()) return 0;

	alg_list_blue  = NULL;
	alg_list_red   = NULL;
	alg_list_green = NULL;
	alg_list_alpha = NULL;

	return 1;
}

int llFillColorMap::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;

	RegisterValue("-algblue",   &alg_list_blue);
	RegisterValue("-algred",    &alg_list_red);
	RegisterValue("-alggreen",  &alg_list_green);
	RegisterValue("-algalpha",  &alg_list_alpha);
	
	return 1;
}

int llFillColorMap::Exec(void) {
	if (!llMapWorker::Exec()) return 0;

	if (!map->IsColorMap()) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: map is no color map", command_name);
		return 0;
	}

	llAlgCollection *algs_blue = NULL;
	if (Used("-algblue")) {
		algs_blue = _llAlgList()->GetAlgCollection(alg_list_blue);
		if (!algs_blue) {
			_llLogger()->WriteNextLine(-LOG_ERROR, "%s: alg collection '%s' not found for blue channel", command_name, alg_list_blue);
			return 0;
		}
		algs_blue->UpdateMaps();
	}
	llAlgCollection *algs_red = NULL;
	if (Used("-algred")) {
		algs_red = _llAlgList()->GetAlgCollection(alg_list_red);
		if (!algs_red) {
			_llLogger()->WriteNextLine(-LOG_ERROR, "%s: alg collection '%s' not found for red channel", command_name, alg_list_red);
			return 0;
		}
		algs_red->UpdateMaps();
	}
	llAlgCollection *algs_green = NULL;
	if (Used("-alggreen")) {
		algs_green = _llAlgList()->GetAlgCollection(alg_list_green);
		if (!algs_green) {
			_llLogger()->WriteNextLine(-LOG_ERROR, "%s: alg collection '%s' not found for green channel", command_name, alg_list_green);
			return 0;
		}
		algs_green->UpdateMaps();
	}
	llAlgCollection *algs_alpha = NULL;
	if (Used("-algalpha")) {
		algs_alpha = _llAlgList()->GetAlgCollection(alg_list_alpha);
		if (!algs_alpha) {
			_llLogger()->WriteNextLine(-LOG_ERROR, "%s: alg collection '%s' not found for alpha channel", command_name, alg_list_alpha);
			return 0;
		}
		algs_alpha->UpdateMaps();
	}

	unsigned int widthx = map->GetWidthX();
	unsigned int widthy = map->GetWidthY();

	for (unsigned int x=0; x<widthx; x++) {
		for (unsigned int y=0; y<widthy; y++) {
			if (algs_blue) {
				double f_blue = 255. * algs_blue->GetValue(map->GetCoordX(x), map->GetCoordY(y));
				if (f_blue > 255.) f_blue = 255.;
				unsigned char blue = (unsigned char)(f_blue);
				map->SetBlue(x, y, blue);
			} else
				map->SetBlue(x, y, 0);
			if (algs_red) {
				double f_red = 255. * algs_red->GetValue(map->GetCoordX(x), map->GetCoordY(y));
				if (f_red > 255.) f_red = 255.;
				unsigned char red = (unsigned char)(f_red);
				map->SetRed(x, y, red);
			} else
				map->SetRed(x, y, 0);
			if (algs_green) {
				double f_green = 255. * algs_green->GetValue(map->GetCoordX(x), map->GetCoordY(y));
				if (f_green > 255.) f_green = 255.;
				unsigned char green = (unsigned char)(f_green);
				map->SetGreen(x, y, green);
			} else
				map->SetGreen(x, y, 0);
			if (algs_alpha) {
				double f_alpha = 255. * algs_alpha->GetValue(map->GetCoordX(x), map->GetCoordY(y));
				if (f_alpha > 255.) f_alpha = 255.;
				unsigned char alpha = (unsigned char)(f_alpha);
				map->SetAlpha(x, y, alpha);
			} else
				map->SetAlpha(x, y, 0);
		}
	}

	return 1;
}
