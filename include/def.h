#ifndef _LL_DEF_
#define _LL_DEF_

#include <stdio.h>
#include <string.h>

#define sprintf_s(buffer, sizeOfBuffer, format, ...) sprintf(buffer, format, __VA_ARGS__)

#define _stricmp(a, b) strcasecmp(a, b)
#define _strnicmp(n, a, b) strncasecmp(n, a, b)

#define gets_s(a, b) gets(a)

void sscanf_s(const char* x, const char* fmt, int * i) {
    sscanf(x, fmt , i);
}

void sscanf_s(const char* x, const char* fmt, float * i) {
    sscanf(x, fmt , i);
}

int fopen_s(FILE** pFile,
            const char *filename,
            const char *mode) {
    *pFile = fopen (filename, mode );
    if (!pFile) return 1;
    return 0;
}



int strcpy_s(char *strDestination,
	     size_t sizeInBytes,
	     const char *strSource) {
    strcpy(strDestination,strSource);
    return 0;
}


#endif
