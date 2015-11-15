#include "../include/llcelliterator.h"
#include "../include/llmaplist.h"

llCellIterator::llCellIterator() : llMapWorker() {
	SetCommandName("CellIterator");
	num_done = 0;
	cellsize_x = cellsize_y = 0.;
}

int llCellIterator::Prepare(void) {
	if (!llMapWorker::Prepare()) return 0;

	return 1;
}

int llCellIterator::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;

	return 1;
}

int llCellIterator::Exec(void) {
	if (!llMapWorker::Exec()) return 0;

	if (!num_done) {
		//1st call, prepare things
		if (_llUtils()->GetValueF("_cellsize_x"))
			cellsize_x = (float)(*_llUtils()->GetValueF("_cellsize_x"));
		if (_llUtils()->GetValueF("_cellsize_y"))
			cellsize_y = (float)(*_llUtils()->GetValueF("_cellsize_y"));
		if (!cellsize_x) {
			_llLogger()->WriteNextLine(-LOG_WARNING, "%s: _cellsize_x not defined", command_name);
			repeat_worker = false;
			return 0;
		}
		if (!cellsize_y) {
			_llLogger()->WriteNextLine(-LOG_WARNING, "%s: _cellsize_y not defined", command_name);
			repeat_worker = false;
			return 0;
		}

		x_min = (int)floor(_llUtils()->x00/cellsize_x);
		x_max = (int)ceil(_llUtils()->x11/cellsize_x);
		y_min = (int)floor(_llUtils()->y00/cellsize_y);
		y_max = (int)ceil(_llUtils()->y11/cellsize_y);
		n_total = (x_max - x_min + 1)*(y_max - y_min + 1);
		//std::cout << x_min << ":" << x_max << ":" << y_min << ":" <<  y_max << std::endl;
		//std::cout << n_total << std::endl;

		running_x = x_min;
		running_y = y_min;
	}

	_llUtils()->x00 = float(running_x)   * cellsize_x;
	_llUtils()->x11 = float(running_x+1) * cellsize_x;
	_llUtils()->y00 = float(running_y)   * cellsize_y;
	_llUtils()->y11 = float(running_y+1) * cellsize_y;

	_llUtils()->SetValue("_cell_x", double(running_x));
	_llUtils()->SetValue("_cell_y", double(running_y));

	repeat_worker = true;
	num_done++;
	running_x++;
	if (running_x > x_max) {
		running_x = x_min;
		running_y++;
	}

	if (num_done == n_total) {
		num_done = 0;
		repeat_worker = false;
	}

	return 1;
}
