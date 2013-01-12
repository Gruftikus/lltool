#include "..\include\llselectall.h"
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <direct.h>
#include <winreg.h>

//constructor
llSelectAll::llSelectAll() : llMapWorker() {
	SetCommandName("SelectAll");
}

int llSelectAll::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;

	return 1;
}

int llSelectAll::Init(void) {
	llMapWorker::Init();

	_llUtils()->x00 = map->GetX1();
	_llUtils()->x11 = map->GetX2();
	_llUtils()->y00 = map->GetY1();
	_llUtils()->y11 = map->GetY2();

	return 1;
}
