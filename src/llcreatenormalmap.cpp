#include "../include/llcreatenormalmap.h"
#include "../include/llmaplist.h"
#include "../include/llmakederivatives.h"


llCreateNormalMap::llCreateNormalMap() : llMapWorker() {
	SetCommandName("CreateNormalMap");
}

int llCreateNormalMap::Prepare(void) {
	if (!llMapWorker::Prepare()) return 0;

	targetname = NULL;
	northboost = boost = 1.0f;
	lodshadows = 0;

	resolution  = 4; 
	range       = 32;
	total_range = 1024;

	return 1;
}

int llCreateNormalMap::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;

	RegisterValue("-name",       &targetname);
	RegisterValue("-northboost", &northboost);
	RegisterValue("-boost",      &boost);

	RegisterValue("-resolution",  &resolution);
	RegisterValue("-range",       &range);
	RegisterValue("-total_range", &total_range);

	RegisterFlag ("-shadows",    &lodshadows);

	return 1;
}

int llCreateNormalMap::Exec(void) {
	if (!llMapWorker::Exec()) return 0;

	if (!Used("-name")) {
		targetname = new char[strlen(mapname)+10];
		sprintf_s(targetname, strlen(mapname)+10, "%s_normal", mapname);
	}

	int widthx = map->GetWidthX();
	int widthy = map->GetWidthY();

	llMap *newmap = _llMapList()->GetMap(targetname);

	int hasnewmap = 0;
	if (newmap) {
		_llLogger()->WriteNextLine(-LOG_WARNING,"%s: map %s existing, going to re-use it", command_name, targetname);
		//_llMapList()->DeleteMap(targetname);
	} else {	
		hasnewmap = 1;
		newmap = new llMap(widthx-1, widthy-1, MAP_COLOR);
		newmap->SetEven();
		newmap->SetCoordSystem(map->GetX1(), map->GetY1(), map->GetX2(), map->GetY2(), map->GetZScale());
	}

	if (lodshadows) 
		_llLogger()->WriteNextLine(-LOG_INFO, "Adding fake LOD shadows, 'north flip boost=%f'", northboost);
	else 
		_llLogger()->WriteNextLine(-LOG_INFO, "Contrast z-boost=%f", boost);

	unsigned int x1 = map->GetRawX(_llUtils()->x00);
	unsigned int y1 = map->GetRawY(_llUtils()->y00);
	unsigned int x2 = map->GetRawX(_llUtils()->x11);
	unsigned int y2 = map->GetRawY(_llUtils()->y11);

	float st_array[3] = {0, 0, 0};

	for (unsigned int y=y1; y<=y2; y++) {
		for (unsigned int x=x1; x<=x2; x++) {
			float height = map->GetZ(x,y);

			/*************************************/
			if (lodshadows) {		
				for (int v=0; v<3; v++) {  //loop over direction vectors
					int directx = -1;
					int directy = 0;
					if (v==1) 
						directx = 1;
					if (v==2) {
						directx = 0;
						directy = -1;
					}
					
					unsigned int cur_x = x + directx * resolution;  
					unsigned int cur_y = y + directy * resolution;
					float slope = 0.f;

					int xx = int(cur_x)-int(x);
					int yy = int(cur_y)-int(y);
					int xxyy = xx + yy;
					if (xxyy<0) xxyy = -xxyy;

					while(xxyy < total_range && map->IsInMap(cur_x,cur_y) ) {
						float cur_slope = 0; 
						if (v<2) 
							cur_slope = directx * (map->GetZ(cur_x,cur_y)-height) / (map->GetWidthXPerRaw() * float(xx));
						else
							cur_slope = directy * (map->GetZ(cur_x,cur_y)-height) / (map->GetWidthYPerRaw() * float(yy));
						
						if (cur_slope > slope) slope = cur_slope;
						
						int                      stepsize = resolution * 8;
						if (xxyy < range)        stepsize = resolution;
						else if (xxyy < 2*range) stepsize = resolution * 2;
						else if (xxyy < 4*range) stepsize = resolution * 4;
						
						cur_x += stepsize*directx;
						cur_y += stepsize*directy;
						xx = int(cur_x)-int(x);
						yy = int(cur_y)-int(y);
						xxyy = xx + yy;
						if (xxyy<0) xxyy = -xxyy;
					}
					st_array[v] = slope;
				}
			}			
			/*************************************/
		
			float tilt         =  0.;
			float tilt_morning =  st_array[1];
			float tilt_evening = -st_array[0];

			float n_x00 = (map->GetZ(x+1,y)   - map->GetZ(x,y))   / (map->GetWidthXPerRaw()/boost);
			float n_y00 = (map->GetZ(x,y+1)   - map->GetZ(x,y))   / (map->GetWidthYPerRaw()/boost);
			float n_x11 = (map->GetZ(x+1,y+1) - map->GetZ(x,y+1)) / (map->GetWidthXPerRaw()/boost);
			float n_y11 = (map->GetZ(x+1,y+1) - map->GetZ(x+1,y)) / (map->GetWidthYPerRaw()/boost);

			float n_x = (n_x00 + n_x11)/2.f;
			float n_y = (n_y00 + n_y11)/2.f;

			if (lodshadows) {
				tilt = tilt_morning + tilt_evening;
				n_x += tilt;
				n_y -= sqrt(st_array[0]*st_array[1]) * northboost;
				if (n_y > -st_array[2]) n_y = -st_array[2];
			}

			float norm = sqrt(n_x*n_x + n_y*n_y + 1.f);
			float n_x1a = ((-n_x/norm)*127.f*1.0f + 128.f);
			float n_y1a = ((-n_y/norm)*127.f*1.0f + 128.f);
			float n_z1a = ((1.f/norm) *127.f*1.0f + 128.f);

			//std::cout << n_x << ":" << n_x1a << std::endl;

			float angle = 0.;

			unsigned char n_x1 = (unsigned char)(n_x1a);
			unsigned char n_y1 = (unsigned char)(n_y1a*cos(angle) + n_z1a*sin(angle));
			unsigned char n_z1 = (unsigned char)(-sin(angle)*n_y1a + n_z1a*cos(angle));
				
			unsigned int xn = x;
			unsigned int yn = y;
			if (!hasnewmap) {
				xn = newmap->GetRawX(map->GetCoordX(x));
				yn = newmap->GetRawY(map->GetCoordY(y));
			}
			newmap->SetTupel(xn, yn, n_z1, n_y1, n_x1, 255);
			//newmap->SetBlue (xn, yn, n_z1);
			//newmap->SetGreen(xn, yn, n_y1);
			//newmap->SetRed  (xn, yn, n_x1);
			//newmap->SetAlpha(xn, yn, 255);
			
		}
	}
	
	if (hasnewmap) _llMapList()->AddMap(targetname, newmap, NULL, NULL, NULL, NULL);

	return 1;
}
