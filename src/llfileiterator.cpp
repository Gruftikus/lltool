#include "../include/llfileiterator.h"
#include "../include/llmaplist.h"

#include <windows.h>
#include "Shlwapi.h"
#include <tchar.h>
#pragma comment (lib, "shlwapi.lib")

llFileIterator::llFileIterator() : llWorker() {
	SetCommandName("FileIterator");
	init_done = 0;
}

void llFileIterator::FindFilesRecursively(LPCTSTR lpFolder, LPCTSTR lpFilePattern) {

	//_tprintf_s(_T("%s\n"), lpFolder);

	TCHAR szFullPattern[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	HANDLE hFindFile;

	// first we are going to process any subdirectories
	PathCombine(szFullPattern, lpFolder, _T("*"));
	hFindFile     = FindFirstFile(szFullPattern, &FindFileData);
	if(hFindFile != INVALID_HANDLE_VALUE) {
		do {
			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && recursive) {
				// found a subdirectory; recurse into it
				//_tprintf_s(_T("dir: %s\n"), szFullPattern);
				PathCombine(szFullPattern, lpFolder, FindFileData.cFileName);
				//if (strcmp(lpFolder, szFullPattern) != 0 && strlen(szFullPattern)>strlen(lpFolder)) {
				if ((lstrcmp(FindFileData.cFileName, TEXT(".")) != 0) && (lstrcmp(FindFileData.cFileName, TEXT("..")) != 0)) {
					//skip "back-directory"
					FindFilesRecursively(szFullPattern, lpFilePattern);
				}
			}
		} while(FindNextFile(hFindFile, &FindFileData));
		FindClose(hFindFile);
	}

	// now we are going to look for the matching files
	PathCombine(szFullPattern, lpFolder, lpFilePattern);
	hFindFile = FindFirstFile(szFullPattern, &FindFileData);
	if(hFindFile != INVALID_HANDLE_VALUE) {
		do {
			if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				// found a file; do something with it
				PathCombine(szFullPattern, lpFolder, FindFileData.cFileName);
				file_list.push_back(_llUtils()->NewString(szFullPattern));
				//_tprintf_s(_T("%s\n"), szFullPattern);
			}
		} while(FindNextFile(hFindFile, &FindFileData));
		FindClose(hFindFile);
	}
}

int llFileIterator::Prepare(void) {
	if (!llWorker::Prepare()) return 0;

	directory = pattern = NULL;
	recursive = 0;

	return 1;
}

int llFileIterator::RegisterOptions(void) {
	if (!llWorker::RegisterOptions()) return 0;

	RegisterValue("-directory", &directory);
	RegisterValue("-pattern",   &pattern, LLWORKER_OBL_OPTION);
	RegisterFlag("-recursive",  &recursive);

	return 1;
}

int llFileIterator::Exec(void) {
	if (!llWorker::Exec()) return 0;

	if (!init_done) {
		if (!Used("-directory")) {
			TCHAR NPath[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, NPath);
			FindFilesRecursively(NPath, _T(pattern));
		} else {
			FindFilesRecursively(_T(directory), _T(pattern));
		}
		init_done = 1;
		position  = 0;
	}

	_llUtils()->SetValue("_filename", file_list[position]);
	position++;

	if (file_list.size() == position) 
		repeat_worker = false;
	else 
		repeat_worker = true;

	return 1;
}
