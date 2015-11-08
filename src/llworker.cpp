
#include "../include/llworker.h"
#include "../include/llutils.h"
#include "../include/lllogger.h"

llWorker::llWorker() {
	command_name = NULL;

	name.resize(0);
	flag.resize(0);
	i_value.resize(0);
	f_value.resize(0);
	d_value.resize(0);
	s_value.resize(0);
}

int llWorker::RegisterFlag(const char *_name, int *_flag, int _opt) {
	AddElement();
	name[GetSize()-1] = _name;
	flag[GetSize()-1] = _flag;
	opt[GetSize()-1]  = _opt;
	return 1;
}

int llWorker::RegisterValue(const char *_name, int *_value, int _opt) {
	AddElement();
	name[GetSize()-1]    = _name;
	i_value[GetSize()-1] = _value;
	opt[GetSize()-1]     = _opt;
	return 1;
}

int llWorker::RegisterValue(const char *_name, float *_value, int _opt) {
	AddElement();
	name[GetSize()-1]    = _name;
	f_value[GetSize()-1] = _value;
	opt[GetSize()-1]     = _opt;
	return 1;
}

int llWorker::RegisterValue(const char *_name, double *_value, int _opt) {
	AddElement();
	name[GetSize()-1]    = _name;
	d_value[GetSize()-1] = _value;
	opt[GetSize()-1]     = _opt;
	return 1;
}

int llWorker::RegisterValue(const char *_name, char **_value, int _opt) {
	AddElement();
	name[GetSize()-1]    = _name;
	s_value[GetSize()-1] = _value;
	opt[GetSize()-1]     = _opt;
	return 1;
}

int llWorker::CheckFlag(char *_flag) {
	char *myflag = _flag;
	if (strlen(myflag) && myflag[0]=='$') {
		AddElementVar(_llUtils()->NewString(myflag));
		return 1;
	}
	for (unsigned int i=0; i<name.size(); i++) {
		if (_stricmp(myflag, name[i]) == 0) {
			if (flag[i]) {
				flag_cache[i] = 1;
				return 1;
			}
		}
	}
	return 0;
}

int llWorker::CheckValue(char *_value) {
	for (unsigned int i=0; i<name.size(); i++) {
		if (_stricmp(_value, name[i]) == 0) {
			used[i] = 1;
			if (i_value[i]) {
				checked_value = 1;
				checked_pos   = i;
				return 1;
			} else if (f_value[i]) {
				checked_value = 2;
				checked_pos   = i;
				return 2;
			} else if (d_value[i]) {
				checked_value = 3;
				checked_pos   = i;
				return 3;
			} else if (s_value[i]) {
				checked_value = 4;
				checked_pos   = i;
				return 3;
			} 
		}
	}
	return 0;
}

int llWorker::AddValue(char *_value) {
	char *dummy = new char[strlen(_value)+1];
	strcpy_s(dummy, strlen(_value)+1, _value);
	_llUtils()->StripQuot(&dummy);
	if (dummy) {
		if (checked_value == 1) {
			i_value_cache[checked_pos] = dummy;
		} else if (checked_value == 2) {
			f_value_cache[checked_pos] = dummy;
		} else if (checked_value == 3) {
			d_value_cache[checked_pos] = dummy;
		} else if (checked_value == 4) {
			s_value_cache[checked_pos] = dummy;
		} 
	} else return 0;
	return checked_value;
}

int llWorker::ReplaceFlags(void) {
	repeat_worker = false;
	int left = 0;

	for (unsigned int j=0; j<flag_cache_var.size(); j++) {
		if (flag_cache_var[j]) {
			flag_cache_var_found[j] = 0;
		}
	}

	for (unsigned int i=0; i<name.size(); i++) {
		if (i_value_cache[i]) {		
			char *dummy = _llUtils()->GetPart(i_value_cache[i], i_value_num[i], &left);
			if (left) {
				i_value_num[i]++;
				repeat_worker = true;
			} else 
				i_value_num[i] = 0;
			_llUtils()->StripSpaces(&dummy);
			_llUtils()->StripQuot(&dummy);
			if (dummy) {
				char *dummy2 = _llUtils()->ReplaceFlags(dummy);
				delete dummy;
				if (dummy2) {
					used[i] = 1;
					sscanf_s(dummy2, "%i", i_value[i]);
					delete dummy2;
				}
			}
		} else if (f_value_cache[i]) {
			char *dummy = _llUtils()->GetPart(f_value_cache[i], f_value_num[i], &left);
			if (left) {
				f_value_num[i]++;
				repeat_worker = true;
			} else 
				f_value_num[i] = 0;
			_llUtils()->StripSpaces(&dummy);
			_llUtils()->StripQuot(&dummy);
			if (dummy) {
				char *dummy2 = _llUtils()->ReplaceFlags(dummy);
				delete dummy;
				if (dummy2) {
					used[i] = 1;
					sscanf_s(dummy2, "%f", f_value[i]);
					delete dummy2;
				}
			}
		} else if (d_value_cache[i]) {
			char *dummy = _llUtils()->GetPart(d_value_cache[i], d_value_num[i], &left);
			if (left) {
				d_value_num[i]++;
				repeat_worker = true;
			} else 
				d_value_num[i] = 0;
			_llUtils()->StripSpaces(&dummy);
			_llUtils()->StripQuot(&dummy);
			if (dummy) {
				char *dummy2 = _llUtils()->ReplaceFlags(dummy);
				delete dummy;
				if (dummy2) {
					used[i] = 1;
					sscanf_s(dummy2, "%lf", d_value[i]);
					delete dummy2;
				}
			}
		} else if (s_value_cache[i]) {
			char *dummy = _llUtils()->GetPart(s_value_cache[i], s_value_num[i], &left);
			if (left) {
				s_value_num[i]++;
				repeat_worker = true;
			} else 
				s_value_num[i] = 0;
			_llUtils()->StripSpaces(&dummy);
			_llUtils()->StripQuot(&dummy);
			if (dummy) {
				char *dummy2 = _llUtils()->ReplaceFlags(dummy);
				delete dummy;
				if (dummy2) {
					used[i] = 1;
					*(s_value[i]) = dummy2; //BUGBUG
				}
			}
		} else if (flag_cache[i]) {
			used[i]    = 1;
			*(flag[i]) = 1;
		} else { 
			for (unsigned int j=0; j<flag_cache_var.size(); j++) {
				if (flag_cache_var[j]) {
					char *var = _llUtils()->ReplaceFlags(flag_cache_var[j]);
					if (strlen(var)) {
						if (_stricmp(name[i], var) == 0) {
							used[i]    = 1;
							*(flag[i]) = 1;
							flag_cache_var_found[j] = 1;
						}
					} else flag_cache_var_found[j] = 1; //switched off, but used
				}
			}
		} 
	}

	for (unsigned int j=0; j<flag_cache_var.size(); j++) {
		if (flag_cache_var[j] && !flag_cache_var_found[j]) {
			_llLogger()->WriteNextLine(-LOG_WARNING, "%s: option [%s] defined by flag [%s] seems to be wrong", command_name, 
				_llUtils()->ReplaceFlags(flag_cache_var[j]), flag_cache_var[j]);
			return 0;
		}
	}

	return 1;
}

int llWorker::Used(const char *_flag) {
	for (unsigned int i=0; i<name.size(); i++) {
		if (_stricmp(_flag, name[i]) == 0) {
			return used[i];
		}
	}
	return 0;
}

int llWorker::RegisterOptions(void) {
	RegisterFlag ("-silent", &silent);
	return 1;
}

int llWorker::Prepare(void) {
	silent = 0;
	for (unsigned int i=0; i<name.size(); i++) {
		used[i] = 0;
	}
	return 1;
}

int llWorker::Exec(void) {
	for (unsigned int i=0; i<name.size(); i++) {
		if ((opt[i] & LLWORKER_OBL_OPTION) && used[i] == 0) {
			_llLogger()->WriteNextLine(-LOG_WARNING, "%s: obligatory option [%s] missing", command_name, name[i]);
			return 0;
		}
	}
	return 1;
}

void llWorker::Print(void) {

	if (silent) return;

	_llLogger()->WriteNextLine(LOG_COMMAND, "%s", GetCommandName());

	for (unsigned int i=0; i<name.size(); i++) {
		if (used[i]) {
			if (i_value_cache[i]) {
 				_llLogger()->AddToLine(' ');
 				_llLogger()->AddToLine(name[i]);
				_llLogger()->AddToLine("=", *(i_value[i]));
			} else if (f_value_cache[i]) {
				_llLogger()->AddToLine(' ');
 				_llLogger()->AddToLine(name[i]);
				_llLogger()->AddToLine("=", *(f_value[i]));
			} else if (d_value_cache[i]) {
				_llLogger()->AddToLine(' ');
 				_llLogger()->AddToLine(name[i]);
				_llLogger()->AddToLine("=", *(d_value[i]));
			} else if (s_value_cache[i] && *(s_value[i])) {
				_llLogger()->AddToLine(' ');
				_llLogger()->AddToLine(name[i]);
				_llLogger()->AddToLine("=\"");
				_llLogger()->AddToLine(*(s_value[i]));
				_llLogger()->AddToLine('\"');
			} else if (flag[i]) {
				_llLogger()->AddToLine(' ');
				_llLogger()->AddToLine(name[i]);
			} 
		}
	}
	_llLogger()->Dump();
}