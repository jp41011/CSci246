/*
 * SMPNode.h
 *
 *  Created on: Nov 27, 2016
 *      Author: juan
 */

#ifndef SMPNODE_H_
#define SMPNODE_H_

#include <iostream>
#include <vector>
#include "CPU.h"
#include "DirEntry.h"

using namespace std;

class SMPNode {
public:
	short nodeID;
	vector<CPU> cpu;
	vector<DirEntry> memory;

	SMPNode(short);

	void printSummary();
	bool loadWord(short, int, int, int&, int&);
	bool loadFromMemory(int, int&);
	void storeWord(short, int, int, int);
	virtual ~SMPNode();
};

#endif /* SMPNODE_H_ */
