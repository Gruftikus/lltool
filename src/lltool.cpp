
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

	int num = argc-1;
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

#ifdef _MSC_VER
	__int64 time_statistics[LLCOM_MAX_WORKERS];
	int time_statistics_cmd[LLCOM_MAX_WORKERS];
	char *time_statistics_cmdname[LLCOM_MAX_WORKERS];
	unsigned int time_statistics_pointer = 0;
#endif

	//******************
	//batch loop
	//******************

	int com = 0;

	mesg->WriteNextLine(LOG_INFO, "****** Go into batch mode in  %s ******", section);

	while (com >-2) {

#ifdef _MSC_VER
		FILETIME idleTime;
		FILETIME kernelTime;
		FILETIME userTime;
		BOOL res = GetSystemTimes( &idleTime, &kernelTime, &userTime );
#endif

#ifdef USE_CATCH
		try {
#endif

			com = batch->GetCommand();
			mesg->Dump();

			//if (batch->CurrentCommand) std::cout << batch->CurrentCommand << std::endl;

#ifdef USE_CATCH
		} catch (char *str) {
			if (batch->CurrentCommand)
				mesg->WriteNextLine(LOG_FATAL, "Catched exception [%s] in [%s]", str, batch->CurrentCommand);
			else 
				mesg->WriteNextLine(LOG_FATAL, "Catched exception [%s]", str);
			DumpExit();
		} catch (int str) {
			if (batch->CurrentCommand)
				mesg->WriteNextLine(LOG_FATAL, "Catched exception [%i] in [%s]", str, batch->CurrentCommand);
			else
				mesg->WriteNextLine(LOG_FATAL, "Catched exception [%i]", str);
			DumpExit();
		} catch (...) {
			if (batch->CurrentCommand)
				mesg->WriteNextLine(LOG_FATAL, "Catched exception in [%s]", batch->CurrentCommand);
			else
				mesg->WriteNextLine(LOG_FATAL, "Catched exception");
			DumpExit();
		}
#endif

#ifdef _MSC_VER
		FILETIME userTime_old = userTime;

		res = GetSystemTimes( &idleTime, &kernelTime, &userTime );

		BOOL found = false;
		for (unsigned int ii=0; ii < time_statistics_pointer;ii++) {
			if (com == time_statistics_cmd[ii]) {
				ULARGE_INTEGER u1 = { userTime.dwLowDateTime, userTime.dwHighDateTime }; 
				ULARGE_INTEGER u2 = { userTime_old.dwLowDateTime, userTime_old.dwHighDateTime }; 
				time_statistics[ii] = u1.QuadPart - u2.QuadPart;
				found = true;
			}
		}
		if (!found && batch->CurrentCommand) {
			ULARGE_INTEGER u1 = { userTime.dwLowDateTime, userTime.dwHighDateTime }; 
			ULARGE_INTEGER u2 = { userTime_old.dwLowDateTime, userTime_old.dwHighDateTime }; 
			time_statistics[time_statistics_pointer] = u1.QuadPart - u2.QuadPart;
			time_statistics_cmd[time_statistics_pointer] = com;
			time_statistics_cmdname[time_statistics_pointer] = new char[strlen(batch->CurrentCommand)+1];
			strcpy_s(time_statistics_cmdname[time_statistics_pointer],strlen(batch->CurrentCommand)+1,batch->CurrentCommand);
			time_statistics_pointer++;
		}
#endif

	}

	std::cout << "****** Batch loop done ******" << std::endl;
#ifdef _MSC_VER
	for (unsigned int ii=0; ii < time_statistics_pointer;ii++) {
		for (unsigned int jj=0; jj < time_statistics_pointer - 1;jj++) {
			if (time_statistics[jj] < time_statistics[jj+1]) {
				__int64 time_statistics_tmp = time_statistics[jj];
				char * time_statistics_cmdname_tmp = time_statistics_cmdname[jj];
				time_statistics[jj] = time_statistics[jj+1];
				time_statistics_cmdname[jj] = time_statistics_cmdname[jj+1];
				time_statistics[jj+1] = time_statistics_tmp;
				time_statistics_cmdname[jj+1] = time_statistics_cmdname_tmp;
			}
		}
	}

	std::cout << "User time per command (sorted):" << std::endl;
	for (unsigned int ii=0; ii < time_statistics_pointer;ii++) {
		if (time_statistics[ii] > 1000000)
			std::cout << time_statistics_cmdname[ii] << ": " << (((double)time_statistics[ii]) /10000000.)<< " s" << std::endl;
	}
#endif


	return 0;

}
