//#include "StdAfx.h"

#include "../include/llcommands.h"
#include "../include/llmaplist.h"

#ifndef _MSC_VER
#include "../include/def.h"
#endif


void llCommands::SetDefaults() {
	mesg     = _llLogger();
	utils    = _llUtils();
	logfile  = NULL;
	skip_next_block = 0;
	lines.resize(0);
	worker_cache.resize(0);
	worker_flags.resize(0);
	sections.resize(0);
	CurrentCommand = "";
	worker_pointer = 0;
	line_pointer   = 0;
	block_level    = 0;
}

//constructor
llCommands::llCommands(FILE *_file, const char *_section) {
	SetDefaults();
	section  = _section;
	file     = _file;
	filename = NULL;
}

//constructor
llCommands::llCommands(const char *_file, const char *_section) {
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

int llCommands::Open(const char *_file, const char *_section) {
	section = _section;

	if (filename) delete filename;

	filename = new char[strlen(_file) + 1];
	strcpy_s(filename, strlen(_file) + 1, _file);
		
	if (file) {
		fclose(file);
		file = NULL;
	}
	
	if (fopen_s(&file, filename, "r")) {
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
		for (unsigned int i=0; i<strlen(dummyline); i++) { //covert DOS files
			if (dummyline[i]=='\r' || dummyline[i]=='\n') dummyline[i]='\0';
		}
		int num = lines.size();
		lines.resize(num+1);
		lines[num] = new char[strlen(dummyline)+1];
		strcpy_s(lines[num], strlen(dummyline)+1, dummyline);
	}

	fclose(file);
	file = NULL;
	return 1;
};

int llCommands::SaveFile(const char *_file) {
	FILE * wfile = NULL;
	if (fopen_s(&wfile, _file, "w")) {
		mesg->WriteNextLine(LOG_ERROR, "Unable to open %s", _file);
		return 0;
	}

	if (gamemode) {
		fprintf(wfile, "[_gamemode]\n");
		fprintf(wfile, "%s -name=\"%s\"\n", LLCOM_GAMEMODE_CMD, game[gamemode]);
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
	fprintf(wfile, "[_saved]\n");
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

int llCommands::Reopen(const char *_section) {
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

int llCommands::CompileScript(void) {
	//int current_section = -1;
	
	for (unsigned int l=0; l<lines.size(); l++) {
		strcpy_s(dummyline, LLCOM_MAX_LINE, lines[l]);
		
		char *linex = dummyline;
		_llUtils()->StripSpaces(&linex);
		_llUtils()->StripComment(linex);
		_llUtils()->StripSpaces(&linex);
		
		if (strlen(linex) > 0) {
			if (linex[0] == '[') {
				char *sec = _llUtils()->NewString(linex);
				sections.push_back(sec);
			} else {
				ExtendWorkerCache();
				
				//check for flags
repeat:				
				if (linex[0] == '@') {
					unsigned int i = _llUtils()->SeekNextSpace(linex);
					if (!i) i = strlen(linex);

					linex[i] = '\0';
					char *delme = _llUtils()->NewString(linex);
					(worker_flags.back()).push_back(delme);

					linex = linex + i + 1;
					_llUtils()->StripSpaces(&linex);
					goto repeat;
				}

				_llUtils()->StripSpaces(&linex);
				if (linex[0] == '{') {
					ExtendWorkerCache();
					bracket_cache[bracket_cache.size()-1] = LLCOM_OPEN_BLOCK;
					linex++;
					_llUtils()->StripSpaces(&linex);
					ExtendWorkerCache();
				} else if (linex[0] == '}') {
					ExtendWorkerCache();
					bracket_cache[bracket_cache.size()-1] = LLCOM_CLOSE_BLOCK;
					linex++;
					_llUtils()->StripSpaces(&linex);	
					ExtendWorkerCache();
				} 

				int com = -1;
				char *ptr, *ptr2;
				char *saveptr1 = NULL, 
					*saveptr2 = NULL;
				ptr = strtok_int(linex, ' ', &saveptr1);
				if (ptr && strlen(ptr)>0) {

					if (_stricmp(ptr, "{") == 0) {
						ExtendWorkerCache();
						bracket_cache[bracket_cache.size()-1] = LLCOM_OPEN_BLOCK;
						if (saveptr1) linex = saveptr1;
						else linex = (char *)"";
						_llUtils()->StripSpaces(&linex);
						ExtendWorkerCache();
						goto repeat;
					}
					if (_stricmp(ptr, "}") == 0) {
						ExtendWorkerCache();
						bracket_cache[bracket_cache.size()-1] = LLCOM_CLOSE_BLOCK;
						if (saveptr1) linex = saveptr1;
						else linex = (char *)"";
						_llUtils()->StripSpaces(&linex);	
						ExtendWorkerCache();
						goto repeat;
					}

					llWorker *worker = NULL;

					for (unsigned int i=0; i<worker_list.size(); i++) {
						if (_stricmp(ptr, worker_list[i]->GetCommandName() ) == 0) {
							com            = i;
							CurrentCommand = worker_list[i]->GetCommandName();
							worker         = worker_list[i]->Clone();
							worker                         ->SetCommandIndex(com);
							worker                         ->RegisterOptions();
							worker                         ->Prepare();
							worker_cache[worker_cache.size()-1] = worker;

							ptr = strtok_int(NULL,' ', &saveptr1);
							while(ptr != NULL) {

								if (_stricmp(ptr, "{") == 0) {
									ExtendWorkerCache();
									bracket_cache[bracket_cache.size()-1] = LLCOM_OPEN_BLOCK;
									if (saveptr1) linex = saveptr1;
									else linex = (char *)"";
									_llUtils()->StripSpaces(&linex);
									ExtendWorkerCache();
									goto repeat;
								}
								if (_stricmp(ptr, "}") == 0) {
									ExtendWorkerCache();
									bracket_cache[bracket_cache.size()-1] = LLCOM_CLOSE_BLOCK;
									if (saveptr1) linex = saveptr1;
									else linex = (char *)"";
									_llUtils()->StripSpaces(&linex);	
									ExtendWorkerCache();
									goto repeat;
								}

								if (!worker->CheckFlag(ptr)) {
									ptr2 = strtok_int(ptr, '=', &saveptr2);
									if (ptr2!=NULL && strlen(ptr2)>0) {
										if (worker->CheckValue(ptr)) {
											ptr2 = strtok_int(NULL, '=', &saveptr2);
											if (ptr2)
												worker->AddValue(ptr2);
											else {
												mesg->WriteNextLine(-LOG_ERROR, "Compile error in line %i", l+1);
												mesg->WriteNextLine(-LOG_ERROR, LLCOM_SYNTAX_ERROR, ptr, CurrentCommand);
											}
										} else {
											mesg->WriteNextLine(-LOG_ERROR, "Compile error in line %i", l+1);
											mesg->WriteNextLine(-LOG_ERROR, LLCOM_UNKNOWN_OPTION, ptr, CurrentCommand);
										}
									}
								}
								ptr = strtok_int(NULL, ' ', &saveptr1);
							}
							goto exit;
						} //if stricmp
					}
exit:
					if (!worker) {
						mesg->WriteNextLine(-LOG_ERROR, "Compile error in line %i", l+1);
						mesg->WriteNextLine(-LOG_ERROR, "--> Unknown command [%s]", ptr);
					}
				} //strlen ptr
			} //not section
		} //if (strlen(linex) > 0) 
	}

	return 1;
}

int llCommands::GetCommand(void) {
	
	if (worker_pointer == worker_cache.size()) {
		return -2; //EOF
	}

	worker_pointer++;

	if (section) {
		if (section_cache[worker_pointer-1] < 0)
			return 0; //no section stored
		if (_stricmp(sections[section_cache[worker_pointer-1]], section)==0) {
			section_is_good = 1;
		} else 
			section_is_good = 0;
		if (!section_is_good) 
			return 0;
	}

	//loop over flags
	unsigned int flagsize = (worker_flags[worker_pointer-1]).size();
	for (unsigned int i=0; i<flagsize; i++) {
		int negative = 0;
		char *flagline = (worker_flags[worker_pointer-1])[i];

		if (strlen(flagline) > 1 && flagline[1] == '!') 
			negative = 1;

		int found = (utils->IsEnabled(flagline+1+negative) == 1 ? 1: 0);

		if (!found && negative==0) {
			skip_next_block = LLCOM_SKIP_BLOCK;
		}
		if (found && negative==1) {
			skip_next_block = LLCOM_SKIP_BLOCK;
		}
	}

	//check for block
	if (bracket_cache[worker_pointer-1] == LLCOM_OPEN_BLOCK) {
		if (block_level == LLCOM_MAX_NESTED_BLOCKS) {
			mesg->WriteNextLine(-LOG_FATAL, "Too many nested blocks (max=%i)", LLCOM_MAX_NESTED_BLOCKS);
		}
		block_skip[block_level] = skip_next_block;
		bracket_reference[block_level] = worker_pointer - 2;
		if (block_level) block_skip[block_level] |= (block_skip[block_level-1] & LLCOM_SKIP_BLOCK);
		skip_next_block = 0;
		block_level++;
		return 0;
	}
	if (bracket_cache[worker_pointer-1] == LLCOM_CLOSE_BLOCK) {
		if (block_level == 0) {
			mesg->WriteNextLine(-LOG_FATAL, "Too many '}': nothing to close here...");
		}
		skip_next_block = 0;
		block_level--;
		if (block_skip[block_level] & LLCOM_ITERATE_BLOCK) {
			if (bracket_reference[block_level] < 0) {
				mesg->WriteNextLine(-LOG_FATAL, "No reference found for closing '}'");
			}
			worker_pointer = bracket_reference[block_level];
		}
		return 0;
	}


	llWorker *worker = worker_cache[worker_pointer - 1];
	if (skip_next_block == LLCOM_SKIP_BLOCK || (block_level && block_skip[block_level-1]==LLCOM_SKIP_BLOCK)) {
		if (worker) {
			skip_next_block = 0;
			//std::cout << "skipped:" << std::endl;
			//worker->ReplaceFlags();
			//worker->Print();
		}	
		return 0;
	}

	int com = -1;

	if (worker) {
		if (skip_next_block & LLCOM_ITERATE_BLOCK) worker_pointer -= 2;
		skip_next_block = 0;
		com = worker->GetCommandIndex();
		worker->Prepare();
	} else return 0;

	//afterburner	
	worker->ReplaceFlags();
	worker->Print();
	int retval = worker->Exec();
	if (worker->Repeat()) {
		skip_next_block |= LLCOM_ITERATE_BLOCK;
	} else if (worker->IsRepeatWorker() && !retval) {
		skip_next_block |= LLCOM_SKIP_BLOCK;
	}
	return com;

}
