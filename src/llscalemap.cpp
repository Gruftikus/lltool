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

	return 1;
}

int llScaleMap::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;

	RegisterValue("-name",   &targetname);
	RegisterValue("-filter", &pFilter);
	RegisterValue("-factor", &factor, LLWORKER_OBL_OPTION);

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
	if (map->IsColorMap()) 
		n = 4;

	if (std::max(widthx, widthy) > RESAMPLER_MAX_DIMENSION) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: size too large (>RESAMPLER_MAX_DIMENSION)", command_name);
		return 0;
	}

	const float filter_scale = 1.0f;//.75f;

	Resampler* resamplers[max_components];
	std::vector<float> samples[max_components];

	// Now create a Resampler instance for each component to process. The first instance will create new contributor tables, which are shared by the resamplers 
	// used for the other components (a memory and slight cache efficiency optimization).
	resamplers[0] = new Resampler(widthx, widthy, newwidthx, newwidthy, Resampler::BOUNDARY_CLAMP, 
		1.0, 0.0f, pFilter, NULL, NULL, filter_scale, filter_scale);
	samples[0].resize(widthx);

	for (int i = 1; i < n; i++) {
		resamplers[i] = new Resampler(widthx, widthy, newwidthx, newwidthy, Resampler::BOUNDARY_CLAMP, 
			1.0f, 0.0f, pFilter, resamplers[0]->get_clist_x(), resamplers[0]->get_clist_y(), filter_scale, filter_scale);
		samples[i].resize(widthx);
	}      

	if (resamplers[0]->status() == 2) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: Bad filter '%s'", command_name, pFilter);
		return 0;
	}

	int running_y = 0;
	for (int yy = 0; yy < widthy; yy++) {
		//const unsigned char* pSrc = &pSrc_image[src_y * src_pitch];

		for (int xx = 0; xx < widthx; xx++) {
			for (int c = 0; c < n; c++) {
				//            if ((c == 3) || ((n == 2) && (c == 1)))
				//             samples[c][x] = *pSrc++ * (1.0f/255.0f);
				//        else
				std::cout << "old: " << map->GetElementRaw(xx, yy) << std::endl;
				samples[c][xx] = map->GetElementRaw(xx, yy);    
			}
		}

		for (int c = 0; c < n; c++) {
			if (!resamplers[c]->put_line(&samples[c][0])) {
				_llLogger()->WriteNextLine(-LOG_FATAL, "%s: Out of memory", command_name);
				return 0;
			}
		}         

		for (;;) {
			int comp_index;
			for (comp_index = 0; comp_index < n; comp_index++) {
				const float *pOutput_samples = resamplers[comp_index]->get_line();
				//std::cout << running_y << std::endl;
				if (!pOutput_samples)
					break;

				for (int xx = 0; xx < newwidthx; xx++) {
					std::cout << pOutput_samples[xx] << std::endl;
					newmap->SetElementRaw(xx, running_y, pOutput_samples[xx]);
				}
			}
			if (comp_index < n)
				break; 

			running_y++;

			if (running_y == newwidthy)
				break; 
		}
	}


#if 0
	for (int y=0; y<widthy; y++) {
		for (int x=0; x<widthx; x++) {
			int x1 = x - dist;
			int x2 = x + dist;
			int y1 = y - dist;
			int y2 = y + dist;
			if (x1 < 0) x1 = 0;
			if (y1 < 0) y1 = 0;
			if (x2 > (int(widthx)-1)) x2 = widthx-1;
			if (y2 > (int(widthy)-1)) y2 = widthy-1;
			float mean=0., num=0.;

			for (int  xx=x1; xx<=x2; xx++) {
				for (int  yy=y1; yy<=y2; yy++) {
					float height = map->GetElementRaw(xx,yy);
//					if (height > minheight) {
						mean +=  height;
						num++;
//					}
				}
			}
			if (num)
				newmap->SetElementRaw(x,y,mean/num);
			else
				//newmap->SetElementRaw(x,y,defaultheight);
				newmap->SetElementRaw(x,y,-15);
		}
	}
#endif

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
