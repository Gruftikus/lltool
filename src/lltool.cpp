
#include <iostream>
#include <math.h>

#ifdef _MSC_VER
#include <windows.h>
#include <direct.h>
//#define USE_CATCH
#else
#include "../include/def.h"
#endif

#include "../include/llutils.h"
#include "../include/llcreateworkers.h"
#include "../include/llcommands.h"
#include "../include/llquadlist.h"

//#define USE_CATCH 

void usage(void) {

	_llLogger()->WriteNextLine(LOG_INFO,"Usage: lltool flag1 flag2 ... [section] filename.mpb");
	_llLogger()->WriteNextLine(LOG_INFO,"       filename: the name of the batch script");
	_llLogger()->WriteNextLine(LOG_INFO,"       flags: the flags which are propagated to the batch script");
	_llLogger()->Dump();

}

void DumpExit(void) {		
	_llLogger()->Dump();
	exit(-1);
}


//************************************************************************************

int main(int argc, char **argv) {

	llLogger   *mesg  = _llLogger();
	llUtils    *utils = _llUtils();
	llCommands *batch = new llCommands();

	std::cout << "Landscape and Heightmap Editing Tool" << std::endl;
	std::cout << "Written by gruftikus@github" << std::endl;
	std::cout << "V0.10 (alpha!), 17.05.2013" << std::endl;
	std::cout << "***********************" << std::endl;

	//******************
	//read the arguments
	//******************

	if (_stricmp(argv[argc-1], "-h") == 0 || _stricmp(argv[argc-1], "--help") == 0) {
		usage();
		DumpExit();
	}

	char *batchname = NULL;

	//check if last option is a filename
	int has_eq=0, has_dot=0;
	if (argc > 1) {
		for (unsigned int i=0; i<strlen(argv[argc-1]); i++) {
			if (argv[argc-1][i] == '.') has_dot++;
			if (argv[argc-1][i] == '=') has_eq++;
		}
	}
	if (has_dot && !has_eq) batchname = argv[argc-1];

	char *section = NULL;

	int num = argc;
	if (batchname) num--;
	for (int i=1; i<num; i++) {
		if (argv[i][0] != '[') {
			char *my_flag_list = new char[strlen(argv[i])+2];
			strcpy_s(my_flag_list, strlen(argv[i])+1, argv[i]);
			utils->AddFlag(my_flag_list);
			mesg->WriteNextLine(LOG_INFO, "Flag: %s", my_flag_list);
		} else
			section = argv[i];
	}

	if (!section) section = "[lltool]";

	mesg->Dump();		
	CreateWorkers(batch);
	mesg->Dump();

	//******************
	//open the batch
	//******************
	if (batchname) {
		if (!batch->Open(batchname, section)) DumpExit();
		batch->ReadCache();
		batch->CompileScript();
	} else {
		batch->ReadStdin(section);
		batch->ReadCache();
		batch->CompileScript();
	}

	_llUtils()->SetValue("_dds_tool", "s3tc.exe");

	_llUtils()->SetValue("_mindistance", "50");
	_llUtils()->SetValue("_cellsize_x",  "1000");
	_llUtils()->SetValue("_cellsize_y",  "1000");

	_llUtils()->SetValue("_quadsize_x",  "5000");
	_llUtils()->SetValue("_quadsize_y",  "5000");
	_llUtils()->SetValue("_quad_levels", "1");

	_llUtils()->x00 = -10000.f;
	_llUtils()->y00 = -10000.f;
	_llUtils()->x11 =  10000.f;
	_llUtils()->y11 =  10000.f;

	return batch->Loop();
}
