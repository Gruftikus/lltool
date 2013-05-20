#include "../include/llscalemap.h"
#include "../include/llmaplist.h"
#include "../include/llmakederivatives.h"

#include "../externals/resampler/resampler.h"


llScaleMap::llScaleMap() : llMapWorker() {
	SetCommandName("ScaleMap");
}

int llScaleMap::Prepare(void) {
	if (!llMapWorker::Prepare()) return 0;

	targetname = NULL;
	pFilter = "blackman";
	source_gamma = 1.75f;

	return 1;
}

int llScaleMap::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;

	RegisterValue("-name",   &targetname);
	RegisterValue("-filter", &pFilter);
	RegisterValue("-factor", &factor, LLWORKER_OBL_OPTION);
	RegisterValue("-gamma",  &source_gamma);

	RegisterFlag ("-rgb",    &rgb);

	return 1;
}

int llScaleMap::Exec(void) {
	if (!llMapWorker::Exec()) return 0;

	if (!Used("-name")) {
		targetname = new char[strlen(mapname)+5];
		sprintf_s(targetname, strlen(mapname)+5, "%s_tmp", mapname);
	}

	llMap *newmap = _llMapList()->GetMap(targetname);

	if (newmap) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: map %s existing", command_name, targetname);
		return 0;
	}

	int widthx = map->GetWidthX();
	int widthy = map->GetWidthY();
	int newwidthx = int(float(map->GetWidthX())*factor);
	int newwidthy = int(float(map->GetWidthY())*factor);

	float defaultheight = map->GetDefaultHeight();
	int   makeshort     = map->GetShort();
	newmap = new llMap(newwidthx, newwidthy, makeshort, defaultheight);

	const int max_components = 4;   
	int n = 1;
	if (map->IsColorMap() || Used("-rgb")) 
		n = 4;

	if (std::max(widthx, widthy) > RESAMPLER_MAX_DIMENSION) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: size too large (>RESAMPLER_MAX_DIMENSION)", command_name);
		return 0;
	}

	const float filter_scale = 1.0f;//.75f;

	// Partial gamma correction looks better on mips. Set to 1.0 to disable gamma correction. 
	//const float source_gamma = 1.75f;
	float srgb_to_linear[256];
	for (unsigned int i = 0; i < 256; i++)
		srgb_to_linear[i] = (float)pow(float(i) * 1.0f/255.0f, source_gamma);
	const int linear_to_srgb_table_size = 4096;
	unsigned char linear_to_srgb[linear_to_srgb_table_size];

	const float inv_linear_to_srgb_table_size = 1.0f / linear_to_srgb_table_size;
	const float inv_source_gamma = 1.0f / source_gamma;

	for (int i = 0; i < linear_to_srgb_table_size; ++i) {
		int k = (int)(255.0f * pow(i * inv_linear_to_srgb_table_size, inv_source_gamma) + .5f);
		if (k < 0) k = 0; else if (k > 255) k = 255;
		linear_to_srgb[i] = (unsigned char)k;
	}

	Resampler* resamplers[max_components];
	std::vector<float> samples[max_components];

	float min = 1.0, max = 0.0;

	if (n==4) {
		min = 0.0; 
		max = 1.0;
	}

	// Now create a Resampler instance for each component to process. The first instance will create new contributor tables, which are shared by the resamplers 
	// used for the other components (a memory and slight cache efficiency optimization).
	resamplers[0] = new Resampler(widthx, widthy, newwidthx, newwidthy, Resampler::BOUNDARY_CLAMP, 
		min, max, pFilter, NULL, NULL, filter_scale, filter_scale);
	samples[0].resize(widthx);

	for (int i = 1; i < n; i++) {
		resamplers[i] = new Resampler(widthx, widthy, newwidthx, newwidthy, Resampler::BOUNDARY_CLAMP, 
			min, max, pFilter, resamplers[0]->get_clist_x(), resamplers[0]->get_clist_y(), filter_scale, filter_scale);
		samples[i].resize(widthx);
	}      

	if (resamplers[0]->status() == 2) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: Bad filter '%s'", command_name, pFilter);
		return 0;
	}

	int running_y = 0;
	for (int yy=0; yy<widthy; yy++) {
		for (int xx=0; xx<widthx; xx++) {
			if (n==1)
				samples[0][xx] = map->GetElementRaw(xx, yy);    
			else {
				unsigned char byte1;
				unsigned char byte2;
				unsigned char byte3;
				unsigned char byte4;
				map->GetTupel(xx, yy, &byte1, &byte2, &byte3, &byte4);
				samples[0][xx] = srgb_to_linear[byte1];   
				samples[1][xx] = srgb_to_linear[byte2]; 
				samples[2][xx] = srgb_to_linear[byte3]; 
				samples[3][xx] = srgb_to_linear[byte4]; 
			}
		}

		for (int c=0; c<n; c++) {
			if (!resamplers[c]->put_line(&samples[c][0])) {
				_llLogger()->WriteNextLine(-LOG_FATAL, "%s: Out of memory", command_name);
				return 0;
			}
		}         

		int do_loop = 1;
		while (do_loop) {
			int c;
			const float *pOutput_samples[4];
			for (c=0; c<n; c++) {
				pOutput_samples[c] = resamplers[c]->get_line();
				if (!pOutput_samples[c])
					do_loop = 0;
			}
			if (do_loop) {
				for (int xx=0; xx<newwidthx; xx++) {
					if (n == 1) 
						newmap->SetElementRaw(xx, running_y, pOutput_samples[0][xx]);
					else {
						int j = (int)(linear_to_srgb_table_size * pOutput_samples[0][xx] + .5f);
						if (j < 0) j = 0; else if (j >= linear_to_srgb_table_size) j = linear_to_srgb_table_size - 1;
						unsigned int byte1 = unsigned int(linear_to_srgb[j]);
						j = (int)(linear_to_srgb_table_size * pOutput_samples[1][xx] + .5f);
						if (j < 0) j = 0; else if (j >= linear_to_srgb_table_size) j = linear_to_srgb_table_size - 1;
						unsigned int byte2 = unsigned int(linear_to_srgb[j]);
						j = (int)(linear_to_srgb_table_size * pOutput_samples[2][xx] + .5f);
						if (j < 0) j = 0; else if (j >= linear_to_srgb_table_size) j = linear_to_srgb_table_size - 1;
						unsigned int byte3 = unsigned int(linear_to_srgb[j]);
						j = (int)(linear_to_srgb_table_size * pOutput_samples[3][xx] + .5f);
						if (j < 0) j = 0; else if (j >= linear_to_srgb_table_size) j = linear_to_srgb_table_size - 1;
						unsigned int byte4 = unsigned int(linear_to_srgb[j]);
						newmap->SetTupel(xx, running_y, byte1, byte2, byte3, byte4);
					}
				}
				running_y++;
				if (running_y == newwidthy)
					break; 
			}
		}
	}

	newmap->SetCoordSystem(map->GetX1(), map->GetY1(), map->GetX2(), map->GetY2(), map->GetZScale());

	llPointList    * points    = _llMapList()->GetPointList(mapname);
	llTriangleList * triangles = _llMapList()->GetTriangleList(mapname);
	llPolygonList  * polygons  = _llMapList()->GetPolygonList(mapname);

	if (Used("-name")) {
		_llMapList()->AddMap(targetname, newmap, points, triangles, polygons);
	} else {
		delete (map);
		_llMapList()->ExchangeMap(mapname, newmap);
	}

	return 1;
}
