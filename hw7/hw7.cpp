/*
============================================================================
 Name        : hw7.cpp
 Author      : Juan Pedraza
 Version     :
 Copyright   :
 Description : CSci 246 - HW 7
 	 Build a simulator for a cc-NUMA architecture with directory-based cache coherence control
 	 (using write-invalidate protocol). In this simulation, please use specific cc-NUMA protocol
 	 used in DASH Machine.
 	 * 4 Nodes total
 	 * 2 CPU per node
	Key(s):
	* InstrID
		- 1 = lw (load word)
		- 2 = sw (store word)
		- 3 = error
	* state
		- 00 = unchanged
		- 01 = shared
		- 10 = dirty / modified

============================================================================
*/

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Global variables for file streams
ofstream outFile;
ifstream inFile;

void getNextInstruction(short&, short&, short&, short&, short&, int&);

int main() {
	cout << "==== Start ====" << endl;
	//cout << "101: " << stoi("101", nullptr, 2) << endl;
	outFile.open("output.txt");
	inFile.open("input.txt");

	short nodeID, cpuID, instrID, rs, rt;
	int offset;

	getNextInstruction(nodeID, cpuID, instrID, rs, rt, offset);
	getNextInstruction(nodeID, cpuID, instrID, rs, rt, offset);
	getNextInstruction(nodeID, cpuID, instrID, rs, rt, offset);
	getNextInstruction(nodeID, cpuID, instrID, rs, rt, offset);

	outFile.close();
	inFile.close();
	cout << "==== End ====" << endl;
	return 0;
}

// This instruction gets the next MIPS instruction from the file and decodes it
// passing the values back by reference
void getNextInstruction(short& outNodeID, short& outCpuID, short& outInstrID, short& outRS, short& outRT, int& outOffset)
{
	string str1, str2; // str1 = node and cpu id, str2 = MIPS instruction
	inFile >> str1; // get the node and cpu
	inFile >> str2; // get the rest, MIPS instruction

	//remove trailing ':' from str1
	cout << str1 << endl;
	str1 = str1.substr(0, str1.length()-1);
	cout << str1 << endl;

	// 2 left most bits = node
	string strNode = str1.substr(0,2);
	string strCPU = str1.substr(2,1);

	// convert and store values as integers
	outNodeID = stoi(strNode,nullptr, 2);
	outCpuID = stoi(strCPU, nullptr, 2);

	string op, rs, rt, offset; // temp variables to split str2 into segments
	op = str2.substr(0, 6);
	rs = str2.substr(6, 5);
	rt = str2.substr(11, 5);
	offset = str2.substr(16);

	if(op == "100011") // lw - load word
		outInstrID = 1;
	else if (op == "101011") // sw - store word
		outInstrID = 2;
	else // error case
		outInstrID = -1; // error

	outRS = stoi(rs, nullptr, 2);
	outRT = stoi(rt, nullptr, 2);
	outOffset = stoi(offset, nullptr, 2);

	cout << op << "\t" << rs << "\t" << rt << "\t" << offset << endl;

	//cout << strNode << endl << strCPU << endl;

	return;
}
