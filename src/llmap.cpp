#include "../include/llmap.h"

//constructor
llMap::llMap(unsigned int _x, unsigned int _y, int _makeshort, float _default) {

	widthx   = _x;
	widthy   = _y;
	f_widthx = float(_x);
	f_widthy = float(_y);
	mesg     = _llLogger();
	sdata    = NULL;
	idata    = NULL;

	is_color_map = 0;
	makeshort    = _makeshort;
	if (makeshort == 0 || makeshort == 1)
		sdata = new llShortarray(widthx*widthy, makeshort, _default); 
	else {
		is_color_map = 1;
		idata = new unsigned int[widthx*widthy];
	}

	scaling = 1;
	uneven  = 1;
	defaultheight = _default;

	InitRnd(0, 0, widthx-1, widthy-1);
}

llMap::llMap(unsigned int _x, unsigned int _y, llShortarray *_data, float _default) {

	widthx   = _x;
	widthy   = _y;
	f_widthx = float(_x);
	f_widthy = float(_y);
	mesg     = _llLogger();

	makeshort = 0;
	sdata     = _data;
	idata     = NULL;

	scaling = 1;
	uneven  = 1;
	defaultheight = _default;
	is_color_map  = 0;

	InitRnd(0, 0, widthx-1, widthy-1);

}

llMap * llMap::Clone(int _expand, int _makeshort) {
	llMap *dummy = new llMap(widthx*_expand, widthy*_expand, makeshort, defaultheight);
	dummy->SetCoordSystem(x1, y1, x2, y2, z);
	return dummy;
}

llMap::~llMap() {
    if (sdata) delete sdata; 
	if (idata) delete idata; 
}

void llMap::InitRnd(unsigned int _x1, unsigned int _y1, unsigned int _x2, unsigned int _y2) {
	rnd_x1 = _x1;
	rnd_x2 = _x2;
	rnd_y1 = _y1;
	rnd_y2 = _y2;
	rnd_widthx = _x2 - _x1;
	rnd_widthy = _y2 - _y1;

	rndx = rnd_widthx;
	rndy = rnd_widthy;

	bool flagx = false;
	bool flagy = false;
	for (int i=31; i>=0; i--) {
		if (!flagx) {
			unsigned int pax = (rndx >> i) & 0x1;
			if (pax) flagx = true;
		} else
			rndx |= (0x1 << i);
		if (!flagy) {
			unsigned int pay = (rndy >> i) & 0x1;
			if (pay) flagy = true;
		} else
			rndy |= (0x1 << i);
	}
}

unsigned int llMap::GetRndX() {
loop:
	unsigned int rnd = rand() & rndx; //BUGBUG: works only up to RAND_MAX
	if (rnd > rnd_widthx) goto loop;
	return rnd + rnd_x1;
}

unsigned int llMap::GetRndY() {
loop:
	unsigned int rnd = rand() & rndy;
	if (rnd > rnd_widthy) goto loop;
	return rnd + rnd_y1;
}

llQuadList *llMap::GenerateQuadList(void) {

	float quadsize_x = 0;
	if (_llUtils()->GetValueF("_quadsize_x"))
		quadsize_x = (float)(*_llUtils()->GetValueF("_quadsize_x"));
	float quadsize_y = 0;
	if (_llUtils()->GetValueF("_quadsize_y"))
		quadsize_y = (float)(*_llUtils()->GetValueF("_quadsize_y"));
	int levels = 0;
	if (_llUtils()->GetValueF("_quad_levels"))
		levels = (int)(*_llUtils()->GetValueF("_quad_levels"));

	if (!quadsize_x) {
		mesg->WriteNextLine(-LOG_ERROR, "'_quadsize_x' not defined, cannot generate the quads ");
		return NULL;
	}
	if (!quadsize_y) {
		mesg->WriteNextLine(-LOG_ERROR, "'_quadsize_y' not defined, cannot generate the quads ");
		return NULL;
	}
	if (!levels) {
		mesg->WriteNextLine(-LOG_ERROR, "'_quad_levels' not defined, cannot generate the quads ");
		return NULL;
	}

	llQuadList *master = NULL;
	llQuadList *last   = NULL;

	while (levels) {

		//std::cout << x1 << ":" << y1 << ":" <<  x2 << ":" << y2 << std::endl;

		int quadx1 = int(floor((x1+.5f) / quadsize_x));
		int quady1 = int(floor((y1+.5f) / quadsize_y));
		int quadx2 = int(floor((x2-.5f) / quadsize_x));
		int quady2 = int(floor((y2-.5f) / quadsize_y));

		int nquads = (quadx2-quadx1+1) * (quady2-quady1+1);
	
		llQuadList *quads= new llQuadList();
		if (!master) {
			_llLogger()->WriteNextLine(-LOG_INFO, "The map covers %i quads at %i level(s)", nquads, levels);
			master = quads;
		}

		for (int ix=quadx1; ix<=quadx2; ix++) {
			for (int iy=quady1; iy<=quady2; iy++) {
				llQuad *myquad = quads->AddQuad(ix, iy, float(ix) * quadsize_x,
					float(iy)   * quadsize_y,
					float(ix+1) * quadsize_x,
					float(iy+1) * quadsize_y);
				if (last) {
					//connect data structures:
					llQuad *lastquad = last->GetQuad   (int(floor(float(ix)/2.0)), int(floor(float(iy)/2.0)));
					int lastquadnum  = last->GetQuadNum(int(floor(float(ix)/2.0)), int(floor(float(iy)/2.0)));
					lastquad->SetSubQuad(ix % 2, iy % 2, myquad);
					llQuadList *master;
					int pos;
					last->AddSubQuad(lastquadnum, myquad, &master, &pos);
					quads->AddMaster(master);
					quads->AddPos(pos);
					//std::cout << "x:" << ix << " y:" << iy << " lastnum:" << lastquadnum << std::endl;
				}
			}
		}
		levels--;
		quadsize_x /= 2.0f;
		quadsize_y /= 2.0f;
		if (last) last->SetSubQuads(quads);
		last = quads;
	}

	return master;
}
