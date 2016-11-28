/*
 * CacheEntry.h
 *
 *  Created on: Nov 27, 2016
 *      Author: juan
 */

#ifndef CACHEENTRY_H_
#define CACHEENTRY_H_

class CacheEntry {
public:
	bool isValid;
	int tag;
	int value;

	CacheEntry();
	virtual ~CacheEntry();
};

#endif /* CACHEENTRY_H_ */
