

//#include "StdAfx.h"

#include "../include/llutils.h"
//#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <time.h>


#ifndef _MSC_VER
#include "../include/def.h"
#else

#ifndef MYMPGUI
#include "../externals/mtparser/MTParserLib/MTParser.h"
#include "../externals/mtparser/MTParserLib/MTParserLocalizer.h"
#endif

#include <iostream>

#include <direct.h>
#include <stdlib.h>
#endif


char *strtok_old(char *string, const char *seps, char **context) {
	//The "original" strtok
	char *head;  /* start of word */
	char *tail;  /* end of word */

	/* If we're starting up, initialize context */
	if (string) {
		*context = string;
	}
	/* Get potential start of this next word */
	head = *context;
	if (head == NULL) {
		return NULL;
	}

	/* Skip any leading separators */
	while (*head && strchr(seps, *head)) {
		head++;
	}

	/* Did we hit the end? */
	if (*head == 0) {
		/* Nothing left */
		*context = NULL;
		return NULL;
	}

	/* skip over word */
	tail = head;
	while (*tail && !strchr(seps, *tail)) {
		tail++;
	}

	/* Save head for next time in context */
	if (*tail == 0) {
		*context = NULL;
	}
	else {
		*tail = 0;
		tail++;
		*context = tail;
	}

	/* Return current word */
	return head;

}

char * strtok_int(char *ptr, const char delim, char **saveptr1) {
	//Wrapper
	int foundquot = 0;
	char *ptr1;
	char delim2[2];
	sprintf_s(delim2, 2, "%c", delim);
	if (*saveptr1) {
		for (unsigned int h=0; h<strlen(*saveptr1); h++) {
			if ((*saveptr1)[h]=='\"' && foundquot) {
				foundquot=0;
				if (h>0 && (*saveptr1)[h-1]=='\\') 
					foundquot=1;
				if (h>1 && (*saveptr1)[h-2]=='\\') 
					foundquot=0;
			} else if ((*saveptr1)[h]=='\"' && !foundquot) {
				foundquot=1;
				if (h>0 && (*saveptr1)[h-1]=='\\') 
					foundquot=0;
				if (h>1 && (*saveptr1)[h-2]=='\\') 
					foundquot=1;
			}
			if ((*saveptr1)[h]==delim && !foundquot) 
				(*saveptr1)[h]='§';
		}
	} 
	foundquot = 0;
	if (ptr) {
		for (unsigned int h=0; h<strlen(ptr); h++) {
			if ((ptr)[h]=='\"' && foundquot) {
				foundquot=0;
				if (h>0 && (ptr)[h-1]=='\\') 
					foundquot=1;
				if (h>1 && (ptr)[h-2]=='\\') 
					foundquot=0;
			} else if ((ptr)[h]=='\"' && !foundquot) {
				foundquot=1;
				if (h>0 && (ptr)[h-1]=='\\') 
					foundquot=0;
				if (h>1 && (ptr)[h-2]=='\\') 
					foundquot=1;
			}
			if ((ptr)[h]==delim && !foundquot) 
				(ptr)[h]='§';
		}
	} 
	ptr1 = strtok_old(ptr, "§", saveptr1);
	foundquot = 0;
	if (*saveptr1) {
		for (unsigned int h=0; h<strlen(*saveptr1); h++) {
			if ((*saveptr1)[h]=='\"' && foundquot) {
				foundquot=0;
				if (h>0 && (*saveptr1)[h-1]=='\\') 
					foundquot=1;
				if (h>1 && (*saveptr1)[h-2]=='\\') 
					foundquot=0;
			} else if ((*saveptr1)[h]=='\"' && !foundquot) {
				foundquot=1;
				if (h>0 && (*saveptr1)[h-1]=='\\') 
					foundquot=0;
				if (h>1 && (*saveptr1)[h-2]=='\\') 
					foundquot=1;
			}
			if ((*saveptr1)[h]=='§' && !foundquot) 
				(*saveptr1)[h]=delim;
		}
	} 
	foundquot = 0;
	if (ptr1) {
		for (unsigned int h=0; h<strlen(ptr1); h++) {
			if ((ptr1)[h]=='\"' && foundquot) {
				foundquot = 0;
				if (h>0 && (ptr1)[h-1]=='\\') 
					foundquot = 1;
				if (h>1 && (ptr1)[h-2]=='\\') 
					foundquot = 0;
					//for (unsigned int j=h-1;j<strlen(ptr1)-1;j++) (ptr1)[j]=(ptr1)[j+1];
					//(ptr1)[strlen(ptr1)-1]='\0';				
			}
			else if ((ptr1)[h]=='\"' && !foundquot) {
				foundquot = 1;
				if (h>0 && (ptr1)[h-1]=='\\') 
					foundquot = 0;
				if (h>1 && (ptr1)[h-2]=='\\') 
					foundquot = 1;
					//for (unsigned int j=h-1;j<strlen(ptr1)-1;j++) (ptr1)[j]=(ptr1)[j+1];
					//(ptr1)[strlen(ptr1)-1]='\0';
			}
			if ((ptr1)[h]=='§' && !foundquot) 
				(ptr1)[h]=delim;
		}
	} 
	return ptr1;
}

//http://www.dateiliste.com/en/source-code/16-cc/577-stristr-case-insensitive-strstr-function.html
char *stristr (char *ch1, const char *ch2)
{
	char  *chN1, *chN2;
	char  *chNdx;
	char  *chRet        = NULL;

	chN1 = strdup (ch1);
	chN2 = strdup (ch2);
	if (chN1 && chN2)
	{
		chNdx = chN1;
		while (*chNdx)
		{
			*chNdx = (char) tolower (*chNdx);
			chNdx ++;
		}
		chNdx = chN2;
		while (*chNdx)
		{
			*chNdx = (char) tolower (*chNdx);
			chNdx ++;
		}
		chNdx = strstr (chN1, chN2);
		if (chNdx)
			chRet = ch1 + (chNdx - chN1);
	}
	free (chN1);
	free (chN2);
	return chRet;
}

int ReadUShort(FILE *fptr,short unsigned *n,int swap)
{
   unsigned char *cptr,tmp;

   if (fread(n,2,1,fptr) != 1)
       return(0);
   if (swap) {
       cptr = (unsigned char *)n;
       tmp = cptr[0];
       cptr[0] = cptr[1];
       cptr[1] =tmp;
   }
   return(1);
}

int ReadShort(FILE *fptr,short signed *n,int swap)
{
   unsigned char *cptr,tmp;

   if (fread(n,2,1,fptr) != 1)
       return(0);
   if (swap) {
       cptr = (unsigned char *)n;
       tmp = cptr[0];
       cptr[0] = cptr[1];
       cptr[1] =tmp;
   }
   return(1);
}

/*
   Write a possibly byte swapped unsigned short integer
*/
int WriteUShort(FILE *fptr,short unsigned n,int swap)
{
   unsigned char *cptr,tmp;

	if (!swap) {
   	if (fwrite(&n,2,1,fptr) != 1)
      	return(0);
   } else {
      cptr = (unsigned char *)(&n);
      tmp = cptr[0];
      cptr[0] = cptr[1];
      cptr[1] =tmp;
      if (fwrite(&n,2,1,fptr) != 1)
         return(0);
   }
   return(1);
}


int ReadUInt(FILE *fptr,unsigned int *n,int swap)
{
   unsigned char *cptr,tmp;

   if (fread(n,4,1,fptr) != 1)
       return(0);
   if (swap) {
       cptr = (unsigned char *)n;
       tmp = cptr[0];
       cptr[0] = cptr[3];
       cptr[3] = tmp;
       tmp = cptr[1];
       cptr[1] = cptr[2];
       cptr[2] = tmp;
   }
   return(1);
}

int ReadInt(FILE *fptr, int *n,int swap)
{
   unsigned char *cptr,tmp;

   if (fread(n,4,1,fptr) != 1)
       return(0);
   if (swap) {
       cptr = (unsigned char *)n;
       tmp = cptr[0];
       cptr[0] = cptr[3];
       cptr[3] = tmp;
       tmp = cptr[1];
       cptr[1] = cptr[2];
       cptr[2] = tmp;
   }

   return(1);
}

int WriteInt(FILE *fptr,int n,int swap)
{
   unsigned char *cptr,tmp;

	if (!swap) {
   	if (fwrite(&n,4,1,fptr) != 1)
      	return(0);
   } else {
      cptr = (unsigned char *)(&n);
      tmp = cptr[0];
      cptr[0] = cptr[3];
      cptr[3] = tmp;
      tmp = cptr[1];
      cptr[1] = cptr[2];
      cptr[2] = tmp;
      if (fwrite(&n,4,1,fptr) != 1)
         return(0);
   }
   return(1);
}

/*
   Write a possibly byte swapped unsigned integer
*/
int WriteUInt(FILE *fptr,unsigned int n,int swap)
{
   unsigned char *cptr,tmp;

   if (!swap) {
      if (fwrite(&n,4,1,fptr) != 1)
         return(0);
   } else {
      cptr = (unsigned char *)(&n);
      tmp = cptr[0];
      cptr[0] = cptr[3];
      cptr[3] = tmp;
      tmp = cptr[1];
      cptr[1] = cptr[2];
      cptr[2] = tmp;
      if (fwrite(&n,4,1,fptr) != 1)
         return(0);
   }
   return(1);
}


llUtils& _fllUtils() {
    static llUtils* ans = new llUtils();
    return *ans;
}

llUtils * _llUtils() {
    return &_fllUtils();
}

//constructor
llUtils::llUtils() {
	num_flags = 0;
	zero      = 0.0;
	one       = 1.0;
	mod_list.resize(0);

	x00 = 0;
	x11 = 1;
	y00 = 0;
	y11 = 1;

	size = 0;
	data = NULL;
}


void llUtils::StripQuot(char **_tmp) {
	if ((*_tmp)[0] == '\"') (*_tmp)++;
	if ((*_tmp)[strlen(*_tmp)-1] == '\"') (*_tmp)[strlen(*_tmp)-1]='\0';
	for (unsigned int h=0; h<strlen(*_tmp); h++) {
		if ((*_tmp)[h]=='\"') { //convert protected \" into "
			if (h>0 && (*_tmp)[h-1]=='\\' && (h==1 || (h>1 && (*_tmp)[h-2]!='\\')) ) {
				for (unsigned int j=h-1; j<strlen(*_tmp)-1; j++) 
					(*_tmp)[j]=(*_tmp)[j+1];
				(*_tmp)[strlen(*_tmp)-1]='\0';
			}
		}
	}
}

void llUtils::StripSpaces(char **_partc) {
	while (**_partc==' ' || **_partc=='\t') (*_partc)++;
	if (strlen(*_partc)) {
		int partend=strlen(*_partc)-1;

		while (((*_partc)[partend]==' ' || (*_partc)[partend]=='\t') && partend>=0) {
			(*_partc)[partend]='\0';
			partend--;
		}
	}
}

void llUtils::StripComment(char *_tmp) {
	bool is_in_quot = false;
	for (unsigned int i=0; i<strlen(_tmp); i++) {
		if (_tmp[i]=='\"') is_in_quot = !is_in_quot;
		if (i && _tmp[i-1]=='\\') is_in_quot = !is_in_quot;
		if (_tmp[i]=='\n') _tmp[i]='\0';
		if ((_tmp[i]==';' || _tmp[i]=='#') && !is_in_quot) _tmp[i]='\0';
	}
}

char *llUtils::ReplaceProtectedKomma(char *_in) {
	for (unsigned int i=0; i<strlen(_in); i++) {
		if (_in[i]=='\\' && _in[i+1]==',') {
			for (unsigned int j=i; j<strlen(_in); j++) {
				_in[j] = _in[j+1];
			}
		}
	}
	return _in;
}

char *llUtils::GetPart(char *_in, int _num, int *_numleft) {
	if (!strlen(_in)) return NewString(_in);
	if (_in[0] != '{' || _in[strlen(_in)-1] != '}') {
		return NewString(_in);
	}
	int start=-1, end=-1;
	if (!_num) start=1;
	for (unsigned int i=0; i<strlen(_in); i++) {
		if (_in[i]==',' && !(i && _in[i-1]=='\\')) {
			//found valid komma
			if (_num) 
				_num--;
			if (!_num) {
				if (start < 0) {
					start = i+1;
				} else if (end < 0) {
					end = i-1;
				} else {
					(*_numleft)++;
				}
			}
		}
	}
	if (end < 0) {
		//no final ,
		end = strlen(_in)-2;
	} else {
		(*_numleft)++;
	}
	if (end<0 || start<0) {
		start = 1;
		end   = strlen(_in)-2;
	}
	char *delme = new char[end-start+2];
	for (int i=start; i<=end; i++) {
		delme[i-start] = _in[i];
	}
	delme[end-start+1] = '\0';
	return ReplaceProtectedKomma(delme);
}

char *llUtils::ReplaceFlags(char *_in) {
	//replaces $flag with its value

	//first copy the string into tmp
	char *tmp = new char[strlen(_in) + 1];
	strcpy_s(tmp, strlen(_in)+1, _in);
	char *linenew = NULL;

check_again:
	//check for flag replacement
	for (unsigned int i=0; i<strlen(tmp); i++) {
		if ((tmp)[i]=='$') {
			if (!(tmp[i+1]=='$') && !(tmp[i+1]=='(') && !(i>0 && tmp[i-1]=='$')) {
				//find the end
				for (unsigned int j=i+1; j<strlen(tmp); j++) {
					if (!(isalnum((tmp)[j]) || (tmp)[j]=='_')) { //find the end
						if ((j-i) <= 1) {
							mesg->WriteNextLine(LOG_ERROR, "Something wrong in [%s]", tmp);
						}
						tmp[i] = '\0';
						char end = (tmp)[j];
						tmp[j] = '\0';
						char *val= (char*)GetValue(tmp + i + 1);
						//sprintf_s(linenew, UTILS_MAX_LINE2, "%s", linex);
						
						if (val) {
							//sprintf_s(linenew, UTILS_MAX_LINE2-strlen(linex), "%s%s", tmp, val);
							if (end != '&') {
								unsigned int len = strlen(tmp) + strlen(val) + 2 + strlen(tmp+j+1);
								linenew = new char[len];
								sprintf_s(linenew, len, "%s%s%c%s", tmp, val, end, tmp+j+1);
							} else {
								unsigned int len = strlen(tmp) + strlen(val) + 1 + strlen(tmp+j+1);
								linenew = new char[len];
								sprintf_s(linenew, len, "%s%s%s", tmp, val, tmp+j+1);
							}
						} else {
							//(if a flag is not existing, we keep it free)	
							if (end != '&') {
								unsigned int len = strlen(tmp) + 2 + strlen(tmp+j+1);
								linenew = new char[len];
								sprintf_s(linenew, len, "%s%c%s", tmp, end, tmp+j+1);
							} else {
								unsigned int len = strlen(tmp) + 1 + strlen(tmp+j+1);
								linenew = new char[len];
								sprintf_s(linenew, len, "%s%s", tmp, tmp+j+1);
							}
						}

#if 0 				
						else {
							if (end != '&') {
								unsigned int len = strlen(tmp) + 7 + 2 + strlen(tmp+j+1);
								linenew = new char[len];
								sprintf_s(linenew, len, "%s<error>%c%s", tmp, end, tmp+j+1);
							} else {
								unsigned int len = strlen(tmp) + 7 + 1 + strlen(tmp+j+1);
								linenew = new char[len];
								sprintf_s(linenew, len, "%s<error>%s", tmp, tmp+j+1);
							}
						}
#endif
						delete tmp;
						tmp = linenew;
						goto check_again;
					}
				}
				tmp[i] = '\0';
				char *val= (char*)GetValue(tmp + i + 1);
				//sprintf_s(linenew, UTILS_MAX_LINE2, "%s", linex);

				if (val) {
					//sprintf_s(linenew, UTILS_MAX_LINE2-strlen(linex), "%s%s", tmp, val);
					unsigned int len = strlen(tmp) + strlen(val) + 1;
					linenew = new char[len];
					sprintf_s(linenew, len, "%s%s", tmp, val);
				} else {
					unsigned int len = strlen(tmp) + 1;
					linenew = new char[len];
					sprintf_s(linenew, len, "%s", tmp);
				}

#if 0
				else {
					unsigned int len = strlen(tmp) + 7 + 1;
					linenew = new char[len];
					sprintf_s(linenew, len, "%s<error>", tmp);
				}
#endif
				delete tmp;
				tmp = linenew;
				goto check_again;
			} //if (!(tmp[i+1]=='$') && !(tmp[i+1]=='(')...
		} //if ((tmp)[i]=='$') {
	} //for...

check_again2:

#ifdef _MSC_VER
	int bracket_position1 = -1;
	int bracket_position2 = 0;
	if (strlen(tmp)>2) {
		for (unsigned int i=0; i<strlen(tmp)-2; i++) {
			if ((tmp)[i]=='$' && (tmp)[i+1]=='(' && bracket_position1<0) {
				int bracket_counter = 0;
				bracket_position1 = i;
				for (unsigned int j=i+1; j<strlen(tmp); j++) {
					if (!bracket_position2 && (tmp)[j]=='(') bracket_counter++;
					if (!bracket_position2 && (tmp)[j]==')') bracket_counter--;
					if (!bracket_position2 && !bracket_counter) bracket_position2 = j;
				}
			}
		}
#ifndef MYMPGUI
		if (bracket_position2) {
			tmp[bracket_position2] = '\0';
			tmp[bracket_position1] = '\0';
			MTParser *parser = new MTParser();
			parser->compile(_T(tmp + bracket_position1 + 2));
			double val = parser->evaluate();
			unsigned int len = strlen(tmp) + strlen(tmp + bracket_position2 + 1) + 20;
			linenew = new char[len];
			if (_llUtils()->GetValue("_flag_format")) {
				char format[1024];
				sprintf_s(format, 1024, "%%s%%%slf%%s", _llUtils()->GetValue("_flag_format"));
				sprintf_s(linenew, len, format, tmp, val, tmp + bracket_position2 + 1);

			} else
				sprintf_s(linenew, len, "%s%lf%s", tmp, val, tmp + bracket_position2 + 1);
			delete tmp;
			tmp = linenew;
			delete parser;
			goto check_again2;
		}
#endif
	}
#endif

	for (unsigned int i=0; i<strlen(tmp); i++) {
		if (i>0 && tmp[i]=='$' && tmp[i-1]=='$') {
			for (unsigned int j=i; j<strlen(tmp); j++) {
				tmp[j]=tmp[j+1];
			}
			i--;
		}
	}

	for (unsigned int i=0; i<strlen(tmp); i++) {
		if (i>0 && tmp[i]=='\\' && tmp[i-1]=='\\') {
			for (unsigned int j=i; j<strlen(tmp); j++) {
				tmp[j]=tmp[j+1];
			}
			//i--;
		}
	}

	return tmp;
}

//taken from http://coding.debuntu.org/c-implementing-str_replace-replace-all-occurrences-substring
char *llUtils::Replace(const char *string, const char *substr, const char *replacement, int num) {

	char *tok = NULL;
	char *newstr = NULL;
	char *oldstr = NULL;
	char *head = NULL;

	/* if either substr or replacement is NULL, duplicate string a let caller handle it */
	if ( substr == NULL || replacement == NULL ) return NewString(string);
	newstr = NewString(string);
	head = newstr;
	while ( (tok = stristr ( head, substr )) && num!=0 ) {
		num--;
		oldstr = newstr;
		newstr = new char[ strlen ( oldstr ) - strlen ( substr ) + strlen ( replacement ) + 1 ];
		/*failed to alloc mem, free old string and return NULL */
		if ( newstr == NULL ){
			free (oldstr);
			return NULL;
		}
		memcpy ( newstr, oldstr, tok - oldstr );
		memcpy ( newstr + (tok - oldstr), replacement, strlen ( replacement ) );
		memcpy ( newstr + (tok - oldstr) + strlen( replacement ), tok + strlen ( substr ), strlen ( oldstr ) - strlen ( substr ) - ( tok - oldstr ) );
		memset ( newstr + strlen ( oldstr ) - strlen ( substr ) + strlen ( replacement ) , 0, 1 );
		/* move back head right after the last replacement */
		head = newstr + (tok - oldstr) + strlen( replacement );
		free (oldstr);
	}
	return newstr;
}

int llUtils::SeekNextSpace(char *_tmp) {
	bool is_in_quot = false;
	for (unsigned int i=0; i<strlen(_tmp); i++) {
		if ((_tmp)[i]=='\"') is_in_quot = !is_in_quot;
		if (i && (_tmp)[i-1]=='\\') is_in_quot = !is_in_quot;
		if (((_tmp)[i]==' ') && !is_in_quot) {
			return i;
		}
	}
	return 0;
}

int llUtils::CrunchStart(char *_s) {
	crunch_string = new char[strlen(_s)+1];
	strcpy_s(crunch_string,strlen(_s)+1,_s);
	crunch_current = NULL;
	crunch_saveptr = NULL;
	return 1;
}

int llUtils::CrunchNext(void) {
	if (!crunch_current) {
		crunch_current = strtok_int(crunch_string, ',', &crunch_saveptr);
		return 1;
	}
	crunch_current = strtok_int(NULL, ',', &crunch_saveptr);
	if (crunch_current) return 1;

	return 0;
}

int llUtils::AddFlag(const char *_name) {
		
	char *val = NULL;
	char *tmp = _llUtils()->NewString(_name);
	
	for (unsigned int i = 0; i < strlen(tmp); i++) {
		if (tmp[i] == '=') {
			tmp[i] = '\0';
			val = tmp + i + 1;
			break;
		}
	}

	if (EnableFlag(tmp)) { //already there
		if (val)
			SetValue(tmp, _llUtils()->NewString(val));
		delete tmp;
		return 1;
	}

	if (num_flags == MAX_FLAGS) {
		mesg->WriteNextLine(LOG_FATAL, "Maximal number of flags (%i) reached, flag %s not added", MAX_FLAGS, _name);
		return 0;
	}

	flag_list[num_flags] = _llUtils()->NewString(tmp);
	delete tmp;
	if (val) 
		flag_value[num_flags] = _llUtils()->NewString(val);
	else
		flag_value[num_flags] = NULL;
	flag_value_f[num_flags]     = 0.0;
	if (val && strlen(val))
		sscanf_s(val, "%lf", &(flag_value_f[num_flags]));
	flag_description[num_flags] = NULL;
	flag_enable[num_flags]      = 1;
	flag_hidden[num_flags]      = 0;

	num_flags++;
	return 1;
}

int llUtils::EnableFlag(const char *_name) {
	//mesg->WriteNextLine(LOG_ERROR,"Enable flag [%s]",myname);
	for (unsigned int i=0; i<num_flags; i++) {
		if (_stricmp(_name, flag_list[i]) == 0) {
			flag_enable[i] = 1;
			return 1;
		}
	}
	return 0;
}

int llUtils::DisableFlag(const char *_name) {
	for (unsigned int i=0; i<num_flags; i++) {
		if (_stricmp(_name, flag_list[i]) == 0) {
			flag_enable[i] = 0;
			return 1;
		}
	}
	return 0;
}

int llUtils::IsEnabled(const char *_name) {
	unsigned int pos=strlen(_name);
	for (unsigned int i=0; i<strlen(_name); i++) {
		if (_name[i] == '=') 
			pos = i;
	}
	for (unsigned int i=0; i<num_flags; i++) {
		if (_strnicmp(_name,flag_list[i],pos)==0 && pos==strlen(flag_list[i])) {
			if (pos == strlen(_name)) {
				return flag_enable[i];
			} else {
				char *val     = (char *)GetValue(flag_list[i]);
				char *newname = new char[strlen(_name+pos+1)+1];			
				strcpy_s(newname, strlen(_name+pos+1)+1, _name+pos+1);
				StripQuot(&newname);
				char *newname2 = ReplaceFlags(newname);
				if (_stricmp(newname2, val) == 0) {	
					delete newname;
					delete newname2;
					return flag_enable[i];
				}
				delete newname;
				delete newname2;
			}
		}
	}
	return 0;
}

int llUtils::SetValue(const char *_name, const char *_value) {
	AddFlag(_name);
	for (unsigned int i=0; i<num_flags; i++) {
		if (_stricmp(_name, flag_list[i]) == 0) {
			if (flag_value[i]) delete (flag_value[i]);
			flag_value[i] = _llUtils()->NewString(_value);
			sscanf_s(_value, "%lf", &(flag_value_f[i]));
			return 1;
		}
	}
	return 0;
}

int llUtils::SetValue(const char *_name, double _value) {
	AddFlag(_name);
	for (unsigned int i=0; i<num_flags; i++) {
		if (_stricmp(_name, flag_list[i]) == 0) {
			char delme[1000];
			if (_llUtils()->GetValue("_flag_format")) {
				char format[1024];
				sprintf_s(format, 1024, "%%%slf", _llUtils()->GetValue("_flag_format"));
				sprintf_s(delme, format, _value);
			} else
				sprintf(delme, "%g", _value);
			if (flag_value[i]) delete (flag_value[i]);
			flag_value[i] = _llUtils()->NewString(delme);
			flag_value_f[i] = _value;
			return 1;
		}
	}
	return 0;
}

int llUtils::DeleteValue(const char *_name) {
	AddFlag(_name);
	for (unsigned int i=0; i<num_flags; i++) {
		if (_stricmp(_name, flag_list[i]) == 0) {
			if (flag_value[i]) {
				delete (flag_value[i]);
				flag_value[i] = NULL;
				return 1;
			}
		}
	}
	return 0;
}

int llUtils::SetHidden(const char *_name) {
	AddFlag(_name);
	for (unsigned int i=0; i<num_flags; i++) {
		if (_stricmp(_name, flag_list[i]) == 0) {
			flag_hidden[i] = 1;
			return 1;
		}
	}
	return 0;
}

const char* llUtils::GetValue(const char *_name) {
	int p = 0;
	if (_stricmp(_name, "_x1") == 0) {
		char tmp[LOG_MAX_LENGTH];
		sprintf_s(tmp, LOG_MAX_LENGTH, "%f", x00); 
		return NewString(tmp);
	} else if (_stricmp(_name, "_x2") == 0) {
		char tmp[LOG_MAX_LENGTH];
		sprintf_s(tmp, LOG_MAX_LENGTH, "%f", x11); 
		return NewString(tmp);
	} else if (_stricmp(_name, "_y1") == 0) {
		char tmp[LOG_MAX_LENGTH];
		sprintf_s(tmp, LOG_MAX_LENGTH, "%f", y00); 
		return NewString(tmp);
	} else if (_stricmp(_name, "_y2") == 0) {
		char tmp[LOG_MAX_LENGTH];
		sprintf_s(tmp, LOG_MAX_LENGTH, "%f", y11); 
		return NewString(tmp);
	} 

	if (_stricmp(_name, "_flaglist") == 0) {
		char tmp[LOG_MAX_LENGTH];
		sprintf_s(tmp, LOG_MAX_LENGTH, "%s", "\0");
		for (unsigned int i=0; i<num_flags; i++) {
			if (flag_enable[i] && (_stricmp(flag_list[i],"_modlist")!=0) && !flag_hidden[i]) {
#ifdef _MSC_VER
				int g = strlen(tmp);
#endif
				if (flag_value[i]) {
					if (p > 0) 
						sprintf_s(tmp, LOG_MAX_LENGTH-g, "%s,%s=%s", tmp, flag_list[i], flag_value[i]); 
					else 
						sprintf_s(tmp, LOG_MAX_LENGTH, "%s=%s", flag_list[i], flag_value[i]); 
					p++;
				} else {
					if (p > 0) 
						sprintf_s(tmp, LOG_MAX_LENGTH-g, "%s,%s", tmp, flag_list[i]); 
					else 
						sprintf_s(tmp, LOG_MAX_LENGTH, "%s", flag_list[i]); 
					p++;
				}
			}
		}
		char *tmp2 = new char[strlen(tmp)+1];  //mem leak, but should not happen too often...
		strcpy_s(tmp2, strlen(tmp)+1, tmp);
		return tmp2;
	}
	for (unsigned int i=0; i<num_flags; i++) {
		if (_stricmp(_name,flag_list[i])==0) {
			if (flag_value[i])  return flag_value[i];
			if (flag_enable[i]) return "1";
			return "0";
		}
	}
	return NULL;
}

double *llUtils::GetValueF(const char *_name) {
//	int p=0;	
	for (unsigned int i=0; i<num_flags; i++) {
		if (_stricmp(_name,flag_list[i]) == 0) {
			if (flag_value[i])  return &(flag_value_f[i]);
			if (flag_enable[i]) return &one;
			return &zero;
		}
	}
	return NULL;
}

int llUtils::SetDescription(const char *_name, char *_value) {
	AddFlag(_name);
	for (unsigned int i=0; i<num_flags; i++) {
		if (_stricmp(_name, flag_list[i]) == 0) {
			flag_description[i] = _value;
			return 1;
		}
	}
	return 0;
}

char* llUtils::GetDescription(const char *_name) {
	for (unsigned int i=0; i<num_flags; i++) {
		if (_stricmp(_name,flag_list[i]) == 0) {
			return flag_description[i];
		}
	}
	return NULL;
}

char * llUtils::GetFlagViaDescription(const char *_value) {
	for (unsigned int i=0; i<num_flags; i++) {
		if (flag_description[i]) {
			if (_stricmp(_value,flag_description[i])==0) {
				return flag_list[i];
			}
		}
	}
	return NULL;
}


int llUtils::WriteFlags(FILE *wfile) {
	for (unsigned int i=0; i<num_flags; i++) {
		if (flag_enable[i] && !flag_hidden[i]) {
			if (flag_value[i]) {
				fprintf(wfile,"SetFlag -name=%s -value=\"%s\"\n", flag_list[i], flag_value[i]);
			} else {
				fprintf(wfile,"SetFlag -name=%s\n", flag_list[i]);
			} 
		} else if (!flag_hidden[i]) {
			if (flag_value[i]) {
				fprintf(wfile,"SetFlag -name=%s -value=\"%s\" -unselect\n", flag_list[i], flag_value[i]);
			} else {
				fprintf(wfile,"SetFlag -name=%s -unselect\n", flag_list[i]);
			} 
		} 
	}

	return 1;
}
