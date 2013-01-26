#ifndef _LL_DEF_
#define _LL_DEF_

#include <stdio.h>
#include <string.h>
#include <cstdio>

#define sprintf_s(buffer, sizeOfBuffer, format, ...) sprintf(buffer, format, __VA_ARGS__)
#define vsprintf_s(buffer, sizeOfBuffer, format, ...) vsprintf(buffer, format, __VA_ARGS__)

#define _stricmp(a, b) strcasecmp(a, b)
#define _strnicmp(n, a, b) strncasecmp(n, a, b)

#define gets_s(a, b) gets(a)

#define _popen(a, b) popen(a, b)

#define sscanf_s(buffer, format, ...) sscanf(buffer, format, __VA_ARGS__)

#define _chdir(a) chdir(a)

#define fopen_s(pFile, filename, mode) ((*(pFile)=fopen(filename, mode))==NULL) ? 1 : 0

#define strcpy_s(strDestination, sizeInBytes, strSource) strcpy(strDestination,strSource)


#endif
