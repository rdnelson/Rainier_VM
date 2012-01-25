/*
 * Utilities.h
 *
 *  Created on: 2012-01-12
 *      Author: daishi
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

class VM;
class Opcode;

class Utilities {
public:
	static char* LoadString(VM* vm, unsigned int address);
	static void LoadOpcodeArgs(Opcode * op, char* mText, unsigned int * registers);
	static int GetArgType(int n, char subcode);

};

#endif /* UTILITIES_H_ */
