/*
 * CPU.cpp
 *
 *  Created on: Nov 27, 2016
 *      Author: juan
 */

#include "CPU.h"

CPU::CPU() {
	// TODO Auto-generated constructor stub
	reg1 = 0;
	reg2 = 0;
	 cache = vector<CacheEntry>(4);
}

CPU::~CPU() {
	// TODO Auto-generated destructor stub
}

