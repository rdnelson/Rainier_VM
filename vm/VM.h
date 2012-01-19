/*
 * VM.h
 *
 *  Created on: 2012-01-11
 *      Author: daishi
 */

#ifndef VM_H_
#define VM_H_

#include "Options.h"
#include "Header.h"
#include "Opcode.h"
#include "Utilities.h"

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
	bool mReady;
	Options* mOpts;
	RNPE_Header mHeader;
	char* mData;
	char* mText;
	unsigned int eax, ebx, ecx, edx, esx, esi, edi, eip, ebp, esp;
};

#endif /* VM_H_ */
