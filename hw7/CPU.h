/*
 * CPU.h
 *
 *  Created on: Nov 27, 2016
 *      Author: juan
 */

#ifndef CPU_H_
#define CPU_H_

#include <vector>

using namespace std;

class CPU {
public:
	int reg1;
	int reg2;
	vector<CacheEntry> cache;

	CPU();
	virtual ~CPU();
};

#endif /* CPU_H_ */
