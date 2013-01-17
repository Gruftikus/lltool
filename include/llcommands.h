#ifndef _PLLCOMMANDS_H_
#define _PLLCOMMANDS_H_

#define LLCOM_MAX_LINE 		10000
#define LLCOM_MAX_LINE2		20000
//#define LLCOM_MAX_FLAGS		2000
#define LLCOM_MAX_GAMES		10

#define LLCOM_MAX_WORKERS	2000

#define LLCOM_SYNTAX_ERROR			"Syntax error in [%s] in [%s]"
#define LLCOM_UNKNOWN_OPTION		"Unknown option [%s] in [%s]"

#define LLCOM_GAMEMODE_CMD "GameMode"

#include <vector>

#include "../include/lllogger.h"
#include "../include/llutils.h"
#include "../include/llworker.h"


class llCommands {

 private:

    FILE     *file;
	FILE     *logfile;
	char     *filename;
	llLogger *mesg;
	llUtils  *utils;

	std::vector<char*>     lines;
	unsigned int           line_pointer;
	std::vector<char*>     sections;
	std::vector<llWorker*> worker_cache;
	unsigned int           worker_pointer;
	std::vector<int>       section_cache;
	std::vector<std::vector<char*>> worker_flags;
	

	char dummyline[LLCOM_MAX_LINE];
	char linenew[LLCOM_MAX_LINE2];
		
	std::vector<llWorker*> worker_list;
	

	char *section;
	int   section_is_good;
	unsigned int current_dump_line;

	int gamemode, noskipinfo, skip_next_command;

	const char *game[LLCOM_MAX_GAMES];
	const char *plugin[LLCOM_MAX_GAMES];
	const char *pattern[LLCOM_MAX_GAMES];
	const char *std_ws[LLCOM_MAX_GAMES];

 public:

    //constructors
    llCommands(FILE *_file, char *_section = NULL);
	llCommands(const char *_file, char *_section = NULL);
	llCommands();

	void SetDefaults(void);

	int Reopen(char *_section);
	int Open(const char *_file, char *_section = NULL);
	int ReadCache(void);
	int ReadStdin(void);
	void ReadStdin(char *_section) {
		section = _section;
	};
	char *GetNextLine(int _cmd);
	int SaveFile(const char *_file);

	int RegisterWorker(llWorker *_worker) {
		if (_worker->GetCommandName()) {
			if (worker_list.size() == LLCOM_MAX_WORKERS) {
				_llLogger()->WriteNextLine(-LOG_FATAL, "LLCOM_MAX_WORKERS reached");
			}
			worker_list.resize(worker_list.size() + 1);
			worker_list[worker_list.size()-1] = _worker;			
		} else {
			_llLogger()->WriteNextLine(-LOG_FATAL, "A worker has no command name");
		}
		return 1;
	};

	int CompileScript(void);
	int GetCommand(void);
	char *CurrentCommand;

	void CloseLogfile(void) {
		if (logfile)
			fclose(logfile);
	}

};

#endif
