#ifndef _PLLCOMMANDS_H_
#define _PLLCOMMANDS_H_

#define LLCOM_MAX_LINE 		10000
#define LLCOM_MAX_LINE2		20000
//#define LLCOM_MAX_FLAGS		2000

#define LLCOM_MAX_WORKERS	2000

#define LLCOM_MAX_NESTED_BLOCKS 10
#define LLCOM_OPEN_BLOCK        1
#define LLCOM_CLOSE_BLOCK       2
#define LLCOM_SKIP_BLOCK        1
#define LLCOM_ITERATE_BLOCK     2


#define LLCOM_SYNTAX_ERROR			"--> Syntax error in [%s] in [%s]"
#define LLCOM_UNKNOWN_OPTION		"--> Unknown option [%s] in [%s]"

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

	int block_skip[LLCOM_MAX_NESTED_BLOCKS];
	unsigned int block_level;

	std::vector<char*>     lines;
	unsigned int           line_pointer;
	std::vector<char*>     sections;
	std::vector<llWorker*> worker_cache;
	unsigned int           worker_pointer;
	std::vector<int>       section_cache;
	std::vector<std::vector<char*> > worker_flags;
	std::vector<int>       bracket_cache;
	std::vector<int>       bracket_reference;

	void ExtendWorkerCache(void) {
		section_cache.push_back(sections.size()-1);
		worker_flags.push_back(std::vector<char*>());
		worker_cache.push_back(NULL);
		bracket_cache.push_back(0);
		bracket_reference.push_back(-1);
	}

	char dummyline[LLCOM_MAX_LINE];
	char linenew[LLCOM_MAX_LINE2];
		
	std::vector<llWorker*> worker_list;
	

	const char  *section;
	int          section_is_good;
	unsigned int current_dump_line;

	int noskipinfo, skip_next_block, noworkerprint;

 public:

	llCommands(FILE *_file, const char *_section = NULL);
	llCommands(const char *_file, const char *_section = NULL);
	llCommands();

	void SetDefaults(void);
	void OpenSection(const char *_section) {
		section = _section;
		worker_pointer = 0;
	};

	int Close(void);
	int Open(const char *_file, const char *_section = NULL);
	int ReadCache(int no_end = 0);
	int ReadStdin(void);
	void ReadStdin(const char *_section) {
		section = _section;
		if (file && file != stdin) {
			fclose(file);
		}
		file = stdin;
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

	int CompileScript(int _compile_all_sections = 1);
	int GetCommand(void);
	const char *CurrentCommand;

	int Loop(void);

	void CloseLogfile(void) {
		if (logfile)
			fclose(logfile);
	}

	void NoWorkerPrint() {noworkerprint = 1;};

};

#endif
