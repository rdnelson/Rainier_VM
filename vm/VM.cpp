/*
 * VM.cpp
 *
 *  Created on: 2012-01-11
 *      Author: daishi
 */

#include "VM.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>

#include "Instruction.h"

#define EVER ;;

Singleton<VM> VM_INSTANCE;

VM::VM() : Memory(0), mReady(false), mOpts(0) {
}

VM::VM(Options *opts) : mOpts(opts) {
	// TODO Auto-generated constructor stub
	if(opts) {
		mReady = opts->IsValid();

	//create memory buffer
	//64k ought to be enough for anyone!
		Memory = new char[mOpts->GetMemSize()]; //64k
	}
	if(mOpts->IsVerbose())
		log.Enable();
	else
		log.Disable();

	log << "VM created. IsReady: " << mReady << std::endl;
}

VM::~VM() {
	// TODO Auto-generated destructor stub
}

void VM::SetOptions(Options* opts)
{
	mOpts = opts;
	mReady = opts->IsValid();
	delete Memory;
	Memory = new char[mOpts->GetMemSize()];
	if(Memory == NULL)
		mReady = false;
	if(mOpts->IsVerbose())
		log.Enable();
	else
		log.Disable();

	log << "Set VM Options." << std::endl;
}

void VM::dump()
{
	for(int i = 0; i < NUM_REGISTERS; i++) {
		log << STR_Registers[i] << ": 0x";
		log << std::hex;
		log << registers[i] << std::dec;
		//if (i % 2)
			std::cerr << "	";
		//else
			std::cerr << std::endl;
	}
}

bool VM::ValidAddress(char* add)
{
	log << "Testing if " << std::hex << (int)add << " is a valid address in array " << (int)Memory << std::dec << std::endl;
	if(add >= Memory)
		if(add < Memory + GetMemSize())
			return true;

	return false;
}

char* VM::GetMemory(unsigned int add)
{
	if(add < GetMemSize())
		return &Memory[add];

	return 0;
}

unsigned int VM::GetRegister(unsigned int reg)
{
	if(reg < NUM_REGISTERS)
		return registers[reg];
	else return 0;
}

void VM::SetRegister(unsigned int reg, unsigned int val)
{
	if(reg < NUM_REGISTERS)
		registers[reg] = val;
}

void VM::Start()
{
	FILE* prog = fopen(mOpts->GetExe().c_str(), "rb");
	if(prog) {
		//get the file length
		fseek(prog, 0, SEEK_END);
		unsigned int mLength = ftell(prog);
		rewind(prog);

		if(mLength - sizeof(RNPE_Header) < GetMemSize()) //if I get paging working this needs to change
		{
			char head[5];
			head[4] = '\0';
			fread(head, 4, sizeof(char), prog);
			rewind(prog);
			RNPE_Header header;
			memset(&header, 0, sizeof(header));

			//if I do base address randomization maybe it goes here?
			if(!strcmp(head, "RNpe")) {
				fread(&header, 1, sizeof(RNPE_Header), prog);
				fread(Memory, mLength - sizeof(RNPE_Header), 1, prog);

				//If I decide to validate the header of the kernel, it should go here

			} else {
				//Raw binary blob
				//Might have to change assembler to deal with this
				fread(Memory, mLength, 1, prog);
			}

			fclose(prog);
			Execute(&header);
		} else {
			fclose(prog);
		}
	}
}


void VM::Execute(RNPE_Header *header)
{
	memset(&registers, 0, sizeof(registers)); //Clear all the registers
	ESP = mOpts->GetMemSize() - 1;
	EBP = ESP;
	unsigned int v;
	memcpy(&v, Memory, 4);
	if(header)
		EIP = header->entry_pos; //it's either initialized or zeroed out

	Instruction* curInst = 0;

	for(EVER) {
		log << "EIP: 0x" << std::hex << EIP << std::dec << std::endl;
		curInst = Instruction::ReadInstruction(EIP);

		if(!curInst) {
			log << "Instruction failed to be created" << std::endl;
			break;
		}

		if(!curInst->IsValid()) {
			log << "SIGILL: Invalid Instruction <" << curInst->GetError() << ">" << std::endl;
			break;
		}

		log << "Incrementing EIP by: " << curInst->GetEipOffset() << std::endl;

		EIP += curInst->GetEipOffset();

		curInst->Execute();
		if(mOpts->IsStepping()) {
			dump();
			getchar();
		}


	}

}
