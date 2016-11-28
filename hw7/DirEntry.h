/*
 * DirEntry.h
 *
 *  Created on: Nov 27, 2016
 *      Author: juan
 */

#ifndef DIRENTRY_H_
#define DIRENTRY_H_

#include <vector>

using namespace std;

enum EntryState
{
	UnCached,
	Shared,
	Dirty
};

class DirEntry {
public:
	short index;
	int value;
	EntryState state;
	vector<bool> isNodeValid;

	DirEntry(short);
	virtual ~DirEntry();
};

#endif /* DIRENTRY_H_ */
