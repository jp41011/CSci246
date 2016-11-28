/*
 * SMPNode.h
 *
 *  Created on: Nov 27, 2016
 *      Author: juan
 */

#ifndef SMPNODE_H_
#define SMPNODE_H_

#include <vector>

using namespace std;

class SMPNode {
public:
	vector<CPU> cpu;

	SMPNode();
	virtual ~SMPNode();
};

#endif /* SMPNODE_H_ */
