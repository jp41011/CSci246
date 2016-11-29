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

Notes:
	* lw $rt, offset($rs)
============================================================================
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <bitset>
#include "SMPNode.h"

using namespace std;

// clock costs
#define LocalCacheCost 1;
#define LocalCPUCost 30;
#define LocalMemCost 100;
#define RemoteNodeCost 135;

// Global variables for file streams
//ofstream outFile;
ifstream inFile;

// vector of nodes
vector<SMPNode> nodes;

// current running total cost
int curCost = 0;
int instrCount = 0; // instruction count to calculate average clock cost

// get the next instruction from file and decode info
void getNextInstruction(string, string, short&, short&, short&, short&, short&, int&);

// execute instruction
void executeInstruction(short, short, short, short, short, int);

// print summary of data in all nodes
void printNodesSummary();

int main() {
	cout << "==== Start ====" << endl;
	//cout << "101: " << stoi("101", nullptr, 2) << endl;

	/* convert int to binary
	string tbin = bitset<32>(8).to_string();
	cout << tbin << endl;
	return 0;
	*/

	for(int i=0; i<4; i++)
	{
		SMPNode tempNode(i);
		nodes.push_back(tempNode);
	}

	printNodesSummary();

	//outFile.open("output.txt");
	inFile.open("input.txt");

	short nodeID, cpuID, instrID, rs, rt;
	int offset;

	string str1, str2;
	while(inFile >> str1) // read in the first part of the line
	{
		inFile >> str2; // read in the rest of the line
		getNextInstruction(str1, str2, nodeID, cpuID, instrID, rs, rt, offset);
		//cout << nodeID << "\t" << cpuID << "\t" << instrID << "\t" << rs << "\t" << rt << "\t" << offset << endl;
		executeInstruction(nodeID, cpuID, instrID, rs, rt, offset);
		instrCount++;
		printNodesSummary();
		//break; // debug
	}

	// print end of program summary stats
	cout << "--------------------" << endl;
	cout << "Total Cost: " << curCost
			<< "\t Instr Count: " << instrCount
			<< "\t Avg Cost: " << double(curCost) / double(instrCount)
			<< endl;

	//outFile.close();
	inFile.close();
	cout << "==== End ====" << endl;
	return 0;
}

// This instruction gets the next MIPS instruction from the file and decodes it
// passing the values back by reference
// str1 and str2 given (read from file before calling this function
void getNextInstruction(string str1, string str2, short& outNodeID, short& outCpuID, short& outInstrID, short& outRS, short& outRT, int& outOffset)
{
	cout << "=========================" << endl;
	cout << "Instruction: " << str1 << " " << str2 << endl;
	/*
	string str1, str2; // str1 = node and cpu id, str2 = MIPS instruction
	inFile >> str1; // get the node and cpu
	inFile >> str2; // get the rest, MIPS instruction
	*/
	//remove trailing ':' from str1
	//cout << str1 << endl;
	str1 = str1.substr(0, str1.length()-1);
	//cout << str1 << endl;

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
	// outRS and outRT are 17 or 18 for registers 1 or 2 ... change them to 1 or 2
	if(outRS != 0)
		outRS -= 16;
	if(outRT != 0)
		outRT -= 16;

	outOffset = stoi(offset, nullptr, 2);

	//cout << op << "\t" << rs << "\t" << rt << "\t" << offset << endl;

	//cout << strNode << endl << strCPU << endl;

	return;
}

// execute instruction
void executeInstruction(short nodeID, short cpuID, short instrID, short rs, short rt, int offset)
{
	int byteOffset = offset/4;
	string binaryOffset = bitset<6>(byteOffset).to_string();
	string tagSTR = binaryOffset.substr(0,4);
	string indexSTR = binaryOffset.substr(4);

	int tag = stoi(tagSTR, nullptr, 2);
	int index = stoi(indexSTR, nullptr, 2);

	cout << "node: " << nodeID << "\tcpu: " << cpuID << endl;
	cout << "tag: " << tag << "\t index: " << index << endl;
	cout << "rt: " << rt << "\t rs: " << rs << endl;

	if(instrID == 1) // load word
	{
		int loadValue = 0; // value to be loaded
		int tempCost = 0; // cost to find
		if(nodes[nodeID].loadWord(cpuID, tag, index, loadValue, tempCost) == true)
		{ // found in the node
			curCost += tempCost;
			nodes[nodeID].storeWord(cpuID, tag, index, loadValue);
		}else // did not find in 1st node have to check other nodes
		{
			curCost += 135;
		}

	}else if (instrID == 2) // store word
	{

	}else{ // error
		cout << "InValid Instruction" << endl;
	}

}

// print summary of data in all nodes
void printNodesSummary()
{
	// print out summary
	for(unsigned int i=0; i < nodes.size(); i++)
		nodes[i].printSummary();

}
