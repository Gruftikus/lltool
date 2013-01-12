

//#include "StdAfx.h"

#include "../include/llcommands.h"
#include "../include/llmaplist.h"

//#include <string.h>
#include <stdio.h>
#include <time.h>


#ifndef _MSC_VER
#include "def.h"
#else

#include <iostream>

#include <direct.h>
#include <stdlib.h>
#endif


void llCommands::SetDefaults() {
	mesg     = _llLogger();
	utils    = _llUtils();
	logfile  = NULL;
	lines.resize(0);
	worker_cache.resize(0);
	CurrentCommand = "";
}

//constructor
llCommands::llCommands(FILE *_file, char *_section) {
	SetDefaults();
	section  = _section;
	file     = _file;
	filename = NULL;
}

//constructor
llCommands::llCommands(const char *_file, char *_section) {
	SetDefaults();
	section  = _section;
	filename = new char[strlen(_file)];
	strcpy_s(filename, strlen(_file) + 1, _file);
	if (fopen_s(&file, filename, "r")) {
		return;
	}
}

llCommands::llCommands() {
	SetDefaults();
	section  = "[None]";
	filename = NULL;
	file     = NULL;
}

int llCommands::Open(const char *_file, char *_section) {
	section = _section;

	if (filename) delete filename;

	filename = new char[strlen(_file) + 1];
	strcpy_s(filename, strlen(_file) + 1, _file);
		
	if (file) {
		fclose(file);
		file = NULL;
	}
	
	if (fopen_s(&file,filename,"r")) {
		mesg->WriteNextLine(LOG_ERROR, "Unable to open %s", filename);
		file = NULL;
		return 0;
	}

	return 1;
}

int llCommands::ReadCache(void) {

	if (!file) {
		_llLogger()->WriteNextLine(LOG_ERROR, "llCommands::ReadCache: no file open");
		return 0;
	}

	while (fgets(dummyline, LLCOM_MAX_LINE, file)) {
		//std::cout << ":" << dummyline << std::endl;
		int num = lines.size();
		lines.resize(num+1);
		worker_cache.resize(num+1);
		lines[num] = new char[strlen(dummyline)+1];
		worker_cache[num] = NULL;
		strcpy_s(lines[num], strlen(dummyline)+1, dummyline);
	}

	fclose(file);
	file = NULL;
	return 1;
};

int llCommands::SaveFile(const char *_file) {
	FILE * wfile;
	if (fopen_s(&wfile,_file,"w")) {
		mesg->WriteNextLine(LOG_ERROR, "Unable to open %s", _file);
		return 0;
	}

	if (gamemode) {
		fprintf(wfile,"[_gamemode]\n");
		fprintf(wfile,"%s -name=\"%s\"\n", LLCOM_GAMEMODE_CMD, game[gamemode]);
	}

	int save=1;
	for (unsigned int i=0; i<lines.size(); i++) {
		//Filter hidden sections:
		if (strlen(lines[i])>1 && _strnicmp(lines[i], "[_",2)==0)
			save = 0;
		else if (strlen(lines[i])>0 && _strnicmp(lines[i],"[",1)==0)
			save = 1;
		if (save)
			fprintf(wfile,"%s", lines[i]);
	}

	//save all variables which are not hidden:
	fprintf(wfile,"[_saved]\n");
	utils->WriteFlags(wfile);

	fclose(wfile);
	return 1;
};

int llCommands::ReadStdin(void) {

	while (gets_s(dummyline, LLCOM_MAX_LINE)) {
		int num = lines.size();
		lines.resize(num+1);
		lines[num] = new char[strlen(dummyline)+1];
		strcpy_s(lines[num], strlen(dummyline)+1, dummyline);
	}

	return 1;
};

char *llCommands::GetNextLine(int _cmd) {
	if (_cmd < 0) {
		current_dump_line = 0;
		return NULL;
	}
	if (current_dump_line >= lines.size()) return NULL;
	current_dump_line++;
	return lines[current_dump_line - 1];
};

int llCommands::Reopen(char *_section) {
	section = _section;

	if (lines.size()) {
		line_pointer = 0;
		return 1;
	}

	if (!filename) return 0;

	if (file) {
		fclose(file);
		file = NULL;
	}
	
	if (fopen_s(&file, filename, "r")) {
		return 0;
	}

	return 1;
}

int llCommands::GetCommand(void) {
	//read the batch file and sets up the command list

	if (lines.size()) {
		if (line_pointer == lines.size()) {
			return -2; //EOF
		}
		strcpy_s(dummyline, LLCOM_MAX_LINE, lines[line_pointer]);
		line_pointer++;
	} else {
		return -2; //EOF
	}
	
	char *linex = dummyline;
	_llUtils()->StripSpaces(&linex);
	_llUtils()->StripComment(linex);
	_llUtils()->StripSpaces(&linex);

	if (strlen(linex)==0) return 0;

	if (section) {
		//seeks for sections
		if (linex[0] == '[') {
			if (_stricmp(linex, section)==0) {
				section_is_good = 1;
			} else 
				section_is_good = 0;
			return 0;
		}
		if (!section_is_good) 
			return 0;
	}

check_again:
	//check for flag replacement
	for (unsigned int i=0; i<strlen(linex); i++) {
		if (linex[i]=='$') {
			if (i==0 || linex[i+1]!='$' || (i>1 && linex[i-1]!='$')) {
				//find the end
				for (unsigned int j=i+1; j<strlen(linex); j++) {
					if (!(isalnum(linex[j]) || linex[j]=='_')) {
						if ((j-i) <= 1) {
							mesg->WriteNextLine(LOG_ERROR, "Something wrong in [%s]", linex);
						}
						//find the end
						linex[i] = '\0';
						char tmp = linex[j];
						linex[j] = '\0';
						sprintf_s(linenew, LLCOM_MAX_LINE2, "%s", linex);
						char *val= (char*)utils->GetValue(linex + i + 1);
						if (val) {
							if (strlen(val) > (LLCOM_MAX_LINE2 - LLCOM_MAX_LINE)) 
								val="<String too long>";
							sprintf_s(linenew, LLCOM_MAX_LINE2-strlen(linex), "%s%s", linenew, val);
							if (tmp != '$') 
								sprintf_s(linenew, LLCOM_MAX_LINE2-strlen(linex)-strlen(val), "%s%c%s", linenew, tmp, linex+j+1);
							else 
								sprintf_s(linenew, LLCOM_MAX_LINE2-strlen(linex)-strlen(val), "%s%s", linenew, linex+j+1);
						}
						//char *bla= new char[strlen(linenew) + 1];
						strcpy_s(linex, LLCOM_MAX_LINE - (linex - dummyline), linenew);
						//linex=bla;
						goto check_again;
					}
				}
				linex[i] = '\0';
				sprintf_s(linenew, LLCOM_MAX_LINE2, "%s",linex);
				char *val=(char *)utils->GetValue(linex + i + 1);
				if (val) {
					if (strlen(val)>(LLCOM_MAX_LINE2 - LLCOM_MAX_LINE)) 
						val="<String too long>";
					sprintf_s(linenew, LLCOM_MAX_LINE2-strlen(linex), "%s%s", linenew, val);
				} 
				//char *bla= new char[strlen(linenew)+i+1];
				strcpy_s(linex, LLCOM_MAX_LINE - (linex - dummyline), linenew);
				//linex=bla;
				goto check_again;
			}
		}
	}

repeat:
	//check for flags
	int negative = 0;
	if (linex[0] == '@') {
		unsigned int i = _llUtils()->SeekNextSpace(linex);
		if (i == strlen(linex)) {
			mesg->WriteNextLine(-LOG_ERROR, "No command after: %s", linex);
			return 0;
		}
		linex[i] = '\0';
		int found = 0;
		if (linex[1] == '!') 
			negative = 1;

		found = (utils->IsEnabled(linex+1+negative) == 1 ? 1: 0);

		if (!found && negative==0) {
			while (*(linex+i+1) == ' ') i++;
			if (!noskipinfo) mesg->WriteNextLine(LOG_INFO, "Flag %s not set, skipped [%s]", linex+1, linex+i+1);
			return 0;
		}
		if (found && negative==1) {
			while (*(linex+i+1) == ' ') i++;
			if (!noskipinfo) mesg->WriteNextLine(LOG_INFO, "Flag %s set, skipped [%s]", linex+2, linex+i+1);
			return 0;
		}
		linex = linex + i + 1;
		goto repeat;
	}

	int com = -1;
	char *ptr, *ptr2;
	char *saveptr1 = NULL, 
		*saveptr2 = NULL;
	
	ptr = strtok_int(linex, ' ', &saveptr1);

	if (!ptr) return 0;
	if (strlen(ptr)==0) return 0;

	llWorker *worker = worker_cache[line_pointer - 1];
	if (!worker) {
		for (unsigned int i=0; i<worker_list.size(); i++) {
			if (_stricmp(ptr, worker_list[i]->GetCommandName() ) == 0) {
				com            = i;
				CurrentCommand = worker_list[i]->GetCommandName();
				worker         = worker_list[i]->Clone();
				worker                         ->SetCommandIndex(com);
				worker                         ->RegisterOptions();
				worker                         ->Prepare();
			}
		}
	} else {
		com = worker->GetCommandIndex();
	}

	if (com==-1 || !worker) {
		mesg->WriteNextLine(LOG_ERROR, "Unknown command [%s]", ptr);
		return com;
	}

	ptr = strtok_int(NULL,' ', &saveptr1);

	while(ptr != NULL) {
		//saveptr2 = NULL;

		if (!worker->CheckFlag(ptr)) {
			ptr2 = strtok_int(ptr, '=',&saveptr2);
			if (ptr2!=NULL && strlen(ptr2)>0) {
				if (worker->CheckValue(ptr)) {
					ptr2 = strtok_int(NULL, '=',&saveptr2);
					if (ptr2)
						worker->AddValue(ptr2);
					else {
						mesg->WriteNextLine(LOG_ERROR, LLCOM_SYNTAX_ERROR, ptr, CurrentCommand);
						return com;
					}
				} else {
					mesg->WriteNextLine(LOG_ERROR, LLCOM_UNKNOWN_OPTION, ptr, CurrentCommand);
				}
			}
		}
		ptr = strtok_int(NULL, ' ', &saveptr1);
	}

	//afterburner
	worker->Print();
	worker->Init();
	return com;

}
