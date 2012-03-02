/*
 * VM.h
 *
 *  Created on: 2012-01-11
 *      Author: daishi
 */

#ifndef VM_H_
#define VM_H_

#include <vector>
#include <sstream>
#include "common/Registers.h"
#include "common/Singleton.hpp"

#include "Options.h"
#include "common/Logger.hpp"
#include "common/Header.h"

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
	VM();
	VM(Options *opts);
	virtual ~VM();

	void SetOptions(Options *opts);

	void Start();

	unsigned int GetMemSize() { return mOpts->GetMemSize(); }
	bool ValidAddress(char* add);

	unsigned int GetRegister(unsigned int reg);
	void SetRegister(unsigned int reg, unsigned int val);

	bool GetFlag(CPU_FLAGS flag) { return mFlags & flag; }
	void SetFlag(CPU_FLAGS flag) { mFlags |= flag; }
	void ClearFlag(CPU_FLAGS flag) { mFlags &= ~flag; }
	void ToggleFlag(CPU_FLAGS flag) { mFlags ^= flag; }

	Logger& GetLogger() { return log; }

	char* GetMemory(unsigned int add);

private:

	//RAM
	char* Memory; //Buffer for entire VM

	void Execute(RNPE_Header* header);


	void dump();

	//

	//State is valid
	bool mReady;

	//initial configuration
	Options* mOpts;

	//CPU Registers
	unsigned int registers[NUM_REGISTERS];
	unsigned char mFlags;

	Logger log;
};

extern Singleton<VM> VM_INSTANCE;

#endif /* VM_H_ */
