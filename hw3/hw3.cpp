/*
============================================================================
 Name        : hw3.cpp
 Author      : Juan Pedraza - 10/2/16
 Description : CSci 246 - Computer Architecture - HW 3
	Build a simulator for the Booth’s 2’s complement number multiplier.
	Will have to implement 1-bit ALU, 16-bit ALU (w/ overflow detection), 16-bit Booth's multiplier.
	Will read in the inputs from an input file.
============================================================================
*/

#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
using namespace std;

//#define BITLENGTH 16 // max number of bits
short const BITLENGTH = 16;

// forward declarations
void ALU1Bit(char, char, char, char, char&, char&);
void ALU16Bit(string, string, char, string&, bool&);
short flipBits(short);
bool isOverflow(string, string, string, char);
string boothsMultiplier(string, string);
void printVar(int, string, string, string, char);

int main() {
	cout << "*** Program Start ***" << endl;
	//string fileName = "input1.txt";
	string fileName = "input2.txt";

	ifstream inFile(fileName.c_str()); // open file

	//initialize
	string val1 = "";
	string val2 = "";

	inFile >> val1;
	inFile >> val2;

	cout << "Value 1: " << val1 << endl << "Value 2: " << val2 << endl;


	string product = boothsMultiplier(val1, val2); // call Booth's Multiplier function and store result

	cout << "Product: " << product << endl; // display product

	inFile.close();
	cout << "*** Program End ***" << endl;
	return 0;
}

/* flipBits(short in)
Input:
	short in - number (1 bit) that will be flipped
Output:
	short in - number whos bits have been negated and all but the right most bit have been masked.
*/
short flipBits(short in)
{
	in = ~in; // flip the bits
	in = in & 1; // keep right most bit
	return in;
}

/*
 * ALU1Bit
 * Inputs:
 *	ai = 1 bit
 *	bi = 1 bit
 *	ci = 1 bit (carry in)
 *	op = operation bit (0 = add, 1 = sub)
 *	result = 1 bit to store the result
 *	carryOut = 1 bit to store the carry out
 * Notes: result and carryOut will be use instead of returning a struct with values
 */
void ALU1Bit(char ai, char bi, char ci, char op, char& result, char& carryOut)
{
	//set temp variables as integers ... easier to manipulate
	short at = (ai == '1' ? 1 : 0);
	short bt = (bi == '1' ? 1 : 0);
	short ct = (ci == '1' ? 1 : 0);

	short rt = 0; // temp result
	short cot = 0; // temp carry out

	if(op == '1') // subtract
	{
		bt = flipBits(bt); // can do that by adding the 2's compliment of the number
	}
	//now continue to the addition

	rt = at ^ bt ^ ct; // draw out table to see that this is true for all possible cases where result would 1
	cot = (at & bt) | (at & ct) | (bt & ct); // there is a carry out if there are at least 2 one bits

	//store results in the output parameters
	result = (rt == 1 ? '1' : '0');
	carryOut = (cot == 1 ? '1' : '0');

	return;
}

/*
void ALU16Bit(string val1, string val2, char op, string& result, bool& overflow)
Input: 
	string val1 - a number represented as a string of bits (binary representation)
	string val2 - 2nd number represented as a string of bits (binary representation)
	char op - represents the operation. 0 = addition, 1 = subtraction
	string& result - where the result of the operation will be stored
	bool& overflow - if operation results in overflow will set this to 1 otherwise set to 0
Output: 
	using "result" and "overflow" parameters to return data
Notes:
	This function will take in the binary representation of 2 numbers and will then do the bitwise addition or subtraction
	of the bits via calls to ALU1bit(...)
 */
void ALU16Bit(string val1, string val2, char op, string& result, bool& overflow)
{
	char carryIn = (op == '1' ? '1' : '0'); // for the special case of sub ... op=1
	string result16 = ""; // string with 16 char (bit) len

	for(int i=BITLENGTH-1; i >= 0; i--) // going from right to left
	{
		char bitResult = ' ';
		char bitCarryOut = ' ';
		char ai = val1[i];
		char bi = val2[i];

		ALU1Bit(ai, bi, carryIn, op, bitResult, bitCarryOut); // call and get result from 1 bit ALU
		carryIn = bitCarryOut; // the carry out becomes the carry in for the next iteration
		result16 = bitResult + result16; // add new bit the left most open spot
	}

	// check for overflow
	overflow = isOverflow(val1, val2, result16, op);
	if(overflow)
	{
		cout << "ERROR: Overflow" << endl;
	}

	result = result16; // move result to the output parameter

	return;
}

/*
bool isOverflow(string val1, string val2, string result, char op)
Input: 
	string val1 - 1st operand
	string val2 - 2nd operand
	string result - result of the operation on the 2 operands
	char op - operation that was performed. 0 = addition, 1 = subtraction
Output:
	true - if there was overflow
	false - if there was NOT overflow
Notes:
	Addition case - if the 2 operands have the same sign and the result has the opposite sign then there was overflow
		- operands have different signs then no overflow possible
	Subtraction case - can be converted to the addition case by multi -1 to the 2nd operand and then follow the Addiction case
 */
bool isOverflow(string val1, string val2, string result, char op)
{
	// get the signs of the 3 numbers (in base 2)
	char sign1 = val1[0];
	char sign2 = val2[0];
	char signResult = result[0];

	if(op == '0') // add
	{
		// if both operands have the same sign and the result has a different sign then overflow
		if(sign1 == sign2 && sign1 != signResult)
			return true; // overflow
		else
			return false; // no overflow if signs are different
	}else{ // op == '1' subtract
		if(sign1 == '0' && sign2 == '1' && signResult == '1')
			return true;
		else if(sign1 == '1' && sign2 == '0' && signResult =='0')
			return true;
		else
			return false; // no overflow
	}

	return false;
}

/*
string boothsMultiplier(string multiplier, string multiplicand)
Input: 
	string multiplier - multiplier number represented as a string of bits (binary 2's complement)
	string multiplicand - multiplicand number represented as a string of bits (binary 2's complement)
Output:
	string - representing the product of the to operands
Notes: 
	performs booth's multiplier and prints out the steps along the way.
 */
string boothsMultiplier(string multiplier, string multiplicand)
{
	//short counter = BITLENGTH; // cycle counter
	string ac = "0000000000000000"; // accumulator
	string mq = multiplier;
	string md = multiplicand;
	char d0 = '0'; // MQ-1 always starts off as 0
					// d1 will be the bit to the left of d0

	//print out table header
	cout << "Counter\t\tMD\t\t\t\tAC\t\t\t\tMQ\t\t\t\tMQ-1" << endl;

	for(int i=BITLENGTH-1; i >= 0; i--)
	{
		printVar(i, md, ac, mq, d0);

		char d1 = mq[mq.length()-1]; // get the right most bit
		bool isOverflow = false; // temp

		if(d1 == '0' && d0 == '1') // Addition
		{
			ALU16Bit(ac, md, '0', ac, isOverflow); // call and get result from 16 bit ALU
			printVar(i, md, ac, mq, d0);
		}else if (d1 == '1' && d0 == '0') // Subtraction
		{
			ALU16Bit(ac, md, '1', ac, isOverflow); // call 16 bit ALU w/ subtract
			printVar(i, md, ac, mq, d0);
		}
		//else 0 && 0 or 1 && 1 either way do nothing

		char signBit = ac[0]; // get the left most bit for sign extension
		char lastAC = ac[ac.length()-1]; // get the right most bit that will be going to MQ

		ac = ac.substr(0,ac.length()-1); // remove the right most bit
		ac = signBit + ac; // sign extend AC

		//char mq_1 = mq[mq.length()-1]; // get the right most bit will be d0 next iteration
		mq = mq.substr(0, mq.length()-1); // remove the right most bit
		mq = lastAC + mq; // move the right most bit from AC to the left most be of MQ

		d0 = d1; // shift the bits to the right

		printVar(i, md, ac, mq, d0);
	}

	// result is AC + MQ
	return ac+mq;
}


/*
void printVar(int counter, string md, string ac, string mq, char d0)
Inputs:
	values that will be printed out in a formatted way.
 */
void printVar(int counter, string md, string ac, string mq, char d0)
{
	string counterBinary = bitset<4>(counter).to_string();
	cout << counterBinary << "\t"
			<< md << "\t\t"
			<< ac << "\t\t"
			<< mq << "\t\t\t"
			<< d0 << endl;
}















