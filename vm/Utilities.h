/*
 * Utilities.h
 *
 *  Created on: 2012-01-12
 *      Author: daishi
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

class VM;

class Utilities {
public:
	static char* LoadString(VM* vm, unsigned int address);

};

#endif /* UTILITIES_H_ */
