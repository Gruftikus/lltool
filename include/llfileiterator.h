#ifndef _PLLFILEITERATOR_H_
#define _PLLFILEITERATOR_H_

#include <windows.h>
#include <tchar.h>

#include "../include/llworker.h"

class llFileIterator : public llWorker {

protected:

	char *directory, *pattern;

	int init_done;
	int position;
	std::vector<char*> file_list;
	void FindFilesRecursively(LPCTSTR lpFolder, LPCTSTR lpFilePattern);

public:

	llFileIterator();

	llWorker * Clone() {
		return new llFileIterator(*this);
	}

	int IsRepeatWorker() {
		return 1;
	}

	int Prepare(void);
	int RegisterOptions(void);
	int Exec(void);

};

#endif
