/*
 * CPU.h
 *
 *  Created on: Nov 27, 2016
 *      Author: juan
 */

#ifndef CPU_H_
#define CPU_H_

#include <iostream>
#include <vector>
#include <bitset>
#include "CacheEntry.h"

using namespace std;

class CPU {
public:
	short cpuID;
	int reg1;
	int reg2;
	vector<CacheEntry> cache;

	CPU(short);
	void printSummary();
	bool loadWord(int, int, int&);
	void storeWord(int, int, int);

	virtual ~CPU();
};

#endif /* CPU_H_ */
