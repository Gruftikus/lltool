#include "../include/llquaditerator.h"
#include "../include/llmaplist.h"

llQuadIterator::llQuadIterator() : llMapWorker() {
	SetCommandName("QuadIterator");
	num_done = 0;
}

int llQuadIterator::Prepare(void) {
	if (!llMapWorker::Prepare()) return 0;

	level = 0;

	return 1;
}

int llQuadIterator::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;

	RegisterValue("-level", &level);

	return 1;
}

int llQuadIterator::Exec(void) {
	if (!llMapWorker::Exec()) return 0;

	if (!points) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: map [%s] has no pointlist", command_name, mapname);
		repeat_worker = false;
		return 0;
	}

	llQuadList *quads = points->GetQuadList();

	if (!quads) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: map [%s] has no quadlist", command_name, mapname);
		repeat_worker = false;
		return 0;
	}

	while(level) {
		quads = quads->GetSubQuads();
		if (!quads) {
			_llLogger()->WriteNextLine(-LOG_ERROR, "%s: map [%s] does not contain the specified level", command_name, mapname);
			repeat_worker = false;
			return 0;
		}
		level--;
	}

	if (num_done >= quads->GetNumQuads()) {
		num_done = 0;
		repeat_worker = false;
		//BUGBUG: skip_worker....
		return 1;
	}

	llQuad *quad = quads->GetQuad(num_done);
	num_done++;

	if (!quad) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: quad not found", command_name);
		repeat_worker = false;
		return 0;
	}

	_llUtils()->x00 = quad->x1;
	_llUtils()->x11 = quad->x2;
	_llUtils()->y00 = quad->y1;
	_llUtils()->y11 = quad->y2;

	
	sprintf_s(myquadx, 1000, "%i", quad->x);
	_llUtils()->SetValue("_quad_x", myquadx);

	sprintf_s(myquady, 1000, "%i", quad->y);
	_llUtils()->SetValue("_quad_y", myquady);

	repeat_worker = true;

	if (num_done == quads->GetNumQuads()) {
		num_done = 0;
		repeat_worker = false;
	}

	return 1;
}
