/*
 * SMPNode.cpp
 *
 *  Created on: Nov 27, 2016
 *      Author: juan
 */

#include "SMPNode.h"

SMPNode::SMPNode(short newID) {
	nodeID = newID;

	CPU cpu0(0);
	CPU cpu1(1);
	cpu.push_back(cpu0);
	cpu.push_back(cpu1);

	short baseIndex = nodeID * 16;

	for(int i=0; i < 16; i++)
	{
		DirEntry tempEntry(baseIndex+i);
		memory.push_back(tempEntry);
	}

}

void SMPNode::printSummary() {
	cout << "--- Node " << nodeID << " Summary ---" << endl;
	// print cpu reg and cache
	for(int i=0; i<cpu.size(); i++)
		cpu[i].printSummary();
	// print mem
	cout << "\tIndex\tMemory\t\t\t\t\tDirectory" << endl;
	for(int i=0; i < memory.size(); i++)
	{
		cout << "\t" << memory[i].index
				<< "\t" << bitset<32>(memory[i].value);
		cout << "\t";
		if(memory[i].state == UnCached)
			cout << "00";
		else if (memory[i].state == Shared)
			cout << "01";
		else if (memory[i].state == Dirty)
			cout << "10";
		else
			cout << "XX"; // error
		cout << " ";

		for(int ii=0; ii < memory[i].isNodeValid.size(); ii++)
		{
			cout << (memory[i].isNodeValid[ii] ? "1" : "0") << " ";
		}

		cout << endl;

	}

}

// try to load from the CPU's cache
bool SMPNode::loadWord(short cpuID, int tag, int index, int& loadValue, int& loadCost)
{
	if(cpu[cpuID].loadWord(tag, index, loadValue) == true)
	{ // found in the local cpu
		loadCost = 1; // LocalCacheCost !!
		return true;
	}
	//else if case look for in other cache on same node
	short nextCpuID = (cpuID == 0 ? 1 : 0); // set the cpu id for the other cpu on same node

	if(cpu[nextCpuID].loadWord(tag, index, loadValue) == true)
	{ // found on the other cache on the same node
		loadCost = 30;
		return true;
	}
	// else case look for it in the node memory
	if(loadFromMemory(index, loadValue) == true)
	{
		loadCost = 100;
		return true;
	}
	// did not find it on this node at all
	loadCost = 0;
	return false;
}

// try to load from the nodes memory blocks
bool SMPNode::loadFromMemory(int index, int& loadValue)
{
	if(memory[0].index <= index && index <= memory[memory.size()-1].index)
	{ // if index is in the domain of this memory block get load it
		short tempIndex = index % 16;
		loadValue = memory[tempIndex].value;

		if(memory[tempIndex].state == UnCached)
		{
			memory[tempIndex].state = Shared; // update to shared
		}
		//todo - if state is dirty
		return true;
	}else
		return false;
}

SMPNode::~SMPNode() {
	// TODO Auto-generated destructor stub
}

