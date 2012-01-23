/*
 * VM.h
 *
 *  Created on: 2012-01-11
 *      Author: daishi
 */

#ifndef VM_H_
#define VM_H_

#include <stack>
#include "Options.h"
#include "Header.h"
#include "Opcode.h"
#include "Utilities.h"

enum CPU_FLAGS {
	FLAG_OFL = 1 << 0,
	FLAG_EQ = 1 << 1,
	FLAG_GT = 1 << 2,
	FLAG_ZERO = 1 << 3,
};

#define NUM_REGISTERS 10

#define EAX registers[0]
#define EBX registers[1]
#define ECX registers[2]
#define EDX registers[3]
#define ESX registers[4]
#define ESI registers[5]
#define EDI registers[6]
#define EBP registers[7]
#define ESP registers[8]
#define EIP registers[9]

class VM {
public:
	VM(Options *opts);
	virtual ~VM();

	void run();
	void Execute();

	Opcode ReadOpcode();
	int ExecuteOpcode(const Opcode & op);
	int Syscall(unsigned char code);

	friend class Utilities;

private:

	int GetOpcodeData(const unsigned int type, const unsigned int val, unsigned int &data);
	bool mReady;
	Options* mOpts;
	RNPE_Header mHeader;
	char* mData;
	char* mText;
	//unsigned int eax, ebx, ecx, edx, esx, esi, edi, eip, ebp, esp;
	unsigned int registers[NUM_REGISTERS];
	unsigned char flags;
	std::stack<unsigned int> mStack;
};

#endif /* VM_H_ */
