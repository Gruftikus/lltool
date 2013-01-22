#include "..\include\llcreatenormalmap.h"
#include "..\include\llmaplist.h"
#include "..\include\llmakederivatives.h"


llCreateNormalMap::llCreateNormalMap() : llMapWorker() {
	SetCommandName("CreateNormalMap");
}

int llCreateNormalMap::Prepare(void) {
	if (!llMapWorker::Prepare()) return 0;

	targetname = NULL;
	northboost = boost = 1.0f;
	lodshadows = 0;

	return 1;
}

int llCreateNormalMap::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;

	RegisterValue("-name",       &targetname);
	RegisterValue("-northboost", &northboost);
	RegisterValue("-boost",      &boost);
	RegisterFlag ("-shadows",    &lodshadows);

	return 1;
}

int llCreateNormalMap::Exec(void) {
	if (!llMapWorker::Exec()) return 0;

	if (!Used("-name")) {
		targetname = new char[strlen(mapname)+10];
		sprintf_s(targetname, strlen(mapname)+10, "%s_normal", mapname);
	}

	llMap *newmap = _llMapList()->GetMap(targetname);

	if (newmap) {
		_llLogger()->WriteNextLine(-LOG_WARNING,"%s: map %s existing, going to delete it", command_name, targetname);
		_llMapList()->DeleteMap(targetname);
	}

	int widthx = map->GetWidthX();
	int widthy = map->GetWidthY();
	newmap = new llMap(widthx-1, widthy-1);
	newmap->SetEven();
	newmap->SetCoordSystem(map->GetX1(), map->GetY1(), map->GetX2(), map->GetY2(), map->GetZScale());


	if (lodshadows) 
		_llLogger()->WriteNextLine(-LOG_INFO, "Adding fake LOD shadows, 'north flip boost=%f'", northboost);
	else 
		_llLogger()->WriteNextLine(-LOG_INFO, "Contrast z-boost=%f", boost);

	unsigned int x1 = map->GetRawX(_llUtils()->x00);
	unsigned int y1 = map->GetRawY(_llUtils()->y00);
	unsigned int x2 = map->GetRawX(_llUtils()->x11);
	unsigned int y2 = map->GetRawY(_llUtils()->y11);

	for (unsigned int y=y1; y<=y2; y++) {
		for (unsigned int x=x1; x<=x2; x++) {
			float height = map->GetZ(x,y);

			float st_array[3];
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
					int resolution = 1; //BUGBUG
					unsigned int cur_x = unsigned int (x + directx * resolution);  
					unsigned int cur_y = unsigned int (y + directy * resolution);
					float slope = 0.f;

					while( (directx*(cur_x-x) + directy*(cur_y-y)) < 1024  && map->IsInMap(cur_x,cur_y) ) {
						float cur_slope = 0; 
						if (v<2) 
							cur_slope = directx * (map->GetZ(cur_x,cur_y)-height) / (map->GetWidthXPerRaw() * float(cur_x-x));
						else
							cur_slope = directy * (map->GetZ(cur_x,cur_y)-height) / (map->GetWidthYPerRaw() * float(cur_y-y));

						if (cur_slope > slope) slope = cur_slope;

						int stepsize = resolution * 8;
						unsigned int range = 128;
						if ((directx*(cur_x-x) + directy*(cur_y-y))<range) stepsize = resolution;
						else if ((directx*(cur_x-x) + directy*(cur_y-y))<2*range) stepsize = resolution * 2;
						else if ((directx*(cur_x-x) + directy*(cur_y-y))<4*range) stepsize = resolution * 4;
						
						cur_x += stepsize*directx;
						cur_y += stepsize*directy;
					}
					st_array[v] = slope;
				}
			}

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

			float angle=0.;

			unsigned char n_x1 = unsigned char(n_x1a);
			unsigned char n_y1 = unsigned char(n_y1a*cos(angle) + n_z1a*sin(angle));
			unsigned char n_z1 = unsigned char(-sin(angle)*n_y1a + n_z1a*cos(angle));

			newmap->SetBlue (x, y, n_z1);
			newmap->SetGreen(x, y, n_y1);
			newmap->SetRed  (x, y, n_x1);
		}
	}
	
	_llMapList()->AddMap(targetname, newmap, NULL, NULL, NULL);

	return 1;
}
