/*
 * VM.h
 *
 *  Created on: 2012-01-11
 *      Author: daishi
 */

#ifndef VM_H_
#define VM_H_

#include <stack>
#include <sstream>
#include "common/Registers.h"
#include "Options.h"
#include "common/Header.h"
#include "common/Opcode.h"
#include "Utilities.h"

enum CPU_FLAGS {
	FLAG_OFL = 1 << 0,
	FLAG_EQUALS = 1 << 1,
	FLAG_GREATER = 1 << 2,
	FLAG_ZERO = 1 << 3,
};


#define EAX registers[REG_EAX]
#define EBX registers[REG_EBX]
#define ECX registers[REG_ECX]
#define EDX registers[REG_EDX]
#define ESI registers[REG_ESI]
#define EDI registers[REG_EDI]
#define EBP registers[REG_EBP]
#define ESP registers[REG_ESP]
#define EIP registers[REG_EIP]

class VM {
public:
	VM(Options *opts);
	virtual ~VM();

	void run();
	void Execute();

	Opcode ReadOpcode();
	int ExecuteOpcode(Opcode & op);
	int Syscall();

	friend class Utilities;

private:

	int GetOpcodeData(const unsigned int type, const unsigned int val, unsigned int &data);
	void ResolveOpcodeArg(Opcode &op, unsigned int arg);
	bool mReady;
	Options* mOpts;
	RNPE_Header mHeader;
	char* mData;
	char* mText;
	//unsigned int eax, ebx, ecx, edx, esx, esi, edi, eip, ebp, esp;
	unsigned int registers[NUM_REGISTERS];
	unsigned char mFlags;
	std::stack<unsigned int> mStack;
	std::stringstream mErr;
	std::streambuf* mCerrBackup;
};

#endif /* VM_H_ */
