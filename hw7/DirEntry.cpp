/*
 * DirEntry.cpp
 *
 *  Created on: Nov 27, 2016
 *      Author: juan
 */

#include "DirEntry.h"

DirEntry::DirEntry(short newIndex) {
	index = newIndex;
	state = UnCached;
	isNodeValid = vector<bool>(4,false);
	value = newIndex + 5; // per requirements

}

DirEntry::~DirEntry() {
	// TODO Auto-generated destructor stub
}

