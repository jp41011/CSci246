/*
 * CPU.cpp
 *
 *  Created on: Nov 27, 2016
 *      Author: juan
 */

#include "CPU.h"

CPU::CPU(short newID) {
	cpuID = newID;
	reg1 = 0;
	reg2 = 0;
	cache = vector<CacheEntry>(4);
}

void CPU::printSummary(){
	cout << "\t CPU " << cpuID << ":" << endl;
	cout << "\t\tRegister(s):" << endl;
	cout << "\t\t S1: " << bitset<32>(reg1) << endl;
	cout << "\t\t S2: " << bitset<32>(reg2) << endl << endl;

	cout << "\t\tCache:" << endl;

	cout << "\t\tIndex\tValid\tTag\tValue" << endl;

	for(unsigned int i=0; i < cache.size(); i++)
	{
		cout << "\t\t " << i
				<< "\t" << (cache[i].isValid ? "1" : "0")
				<< "\t" << bitset<4>(cache[i].tag)
				<< "\t" << bitset<32>(cache[i].value)
				<< endl;
	}

}

// lock for the value using the index and the tag in the current cpu cache
bool CPU::loadWord(int tag, int index, int& loadValue)
{
	if(cache[index].tag == tag && cache[index].isValid)
	{
		loadValue = cache[index].value;
		return true;
	}else
		return false;
}

CPU::~CPU() {
	// TODO Auto-generated destructor stub
}

