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
#include "Header.h"

VM::VM(Options *opts) : mOpts(opts) {
	// TODO Auto-generated constructor stub
	mReady = opts->IsValid();
	std::cerr << "VM created. mReady: " << mReady << std::endl;
}

VM::~VM() {
	// TODO Auto-generated destructor stub
}

void VM::run()
{
	std::cerr << "Running" << std::endl;
	if (!mReady)
		return;
	std::cerr << "Still running" << std::endl;
	std::ifstream fin(mOpts->GetExe().c_str());
	if(!fin) {
		mReady = false;
		return;
	}
	std::cerr << "Executable loaded" << std::endl;

	//populate header
	fin.read((char*)&mHeader, sizeof(RNPE_Header));

	std::cerr << "Header read in" << std::endl;

	//ensure sizes all match up
	if (mHeader.text_size + mHeader.data_size + sizeof(RNPE_Header) != mHeader.size) {
		mReady = false;
		fin.close();
		std::cerr << "Header sizes don't match up" << std::endl;
		return;
	}

	//create data and text buffers
	mData = new char[mHeader.data_size];
	mText = new char[mHeader.text_size];

	std::cerr << "Created mData and mText" << std::endl;

	//load in data and text
	fin.seekg(mHeader.data_pos);
	fin.read(mData, mHeader.data_size);
	unsigned int read_count = fin.gcount();
	if( read_count != mHeader.data_size ) {
		mReady = false;
		std::cerr << "Invalid read size: data" << std::endl;
		fin.close();
		return;
	}
	//if( mHeader.text_pos < mHeader.data_pos 

	fin.seekg(mHeader.text_pos);
	fin.read(mText, mHeader.text_size);
	read_count = fin.gcount();
	std::cerr << "Characters in text: " << read_count << std::endl;
	if( read_count != mHeader.text_size ) {
		mReady = false;
		std::cerr << "Invalid read size: text" << std::endl;
		fin.close();
		return;
	}

	fin.close();

	Execute();

}

void VM::Execute()
{
	std::cerr << "Beginning execution" << std::endl;
	if( mHeader.entry_pos > mHeader.text_size )
		return;

	EIP = mHeader.entry_pos;
	Opcode op;
	int retVal;
	do
	{
		std::cerr << "Reading opcode" << std::endl;
		op = ReadOpcode();
		retVal = ExecuteOpcode(op);
	} while( !retVal );

}

Opcode VM::ReadOpcode()
{
	Opcode tmp;
	tmp.isValid = 1;
	int v = EIP;
	tmp.opcode = mText[v];
	EIP++;
	std::cerr << "Opcode:" << (int)tmp.opcode << std::endl;

	Utilities::LoadOpcodeArgs(&tmp, mText, registers);
	return tmp;
}

int VM::ExecuteOpcode(const Opcode &op)
{
	if (!op.isValid)
		return -3;
	int ret = 0;
	std::cerr << "Executing opcode: " << (int) op.opcode << std::endl;
	switch(op.opcode)
	{
	case MOV_OP: //mov
		if (op.arg1 < NUM_REGISTERS) {
			registers[op.arg1] = op.arg2;
		} else {
			std::cerr << "Invalid Register: " << op.arg1 << std::endl;
			return -1;
		}
		break;
	case PUSH_OP:
		if (op.arg1 < NUM_REGISTERS) {
			mStack.push(registers[op.arg1]);
		} else {
			std::cerr << "Invalid Registers: " << op.arg1 << std::endl;
			return -1;
		}
		break;
	case POP_OP:
		if (op.arg1 < NUM_REGISTERS) {
			registers[op.arg1] = mStack.top();
			mStack.pop();
		} else {
			std::cerr << "Invalid Registers: " << op.arg1 << std::endl;
			return -1;
		}
		break;
	case TEST_OP:
		if (op.arg1 < NUM_REGISTERS && op.arg2 < NUM_REGISTERS) {
			if(registers[op.arg1] == registers[op.arg2]) {
				mFlags |= FLAG_EQUALS;
				mFlags &= ~FLAG_GREATER;
			} else if(registers[op.arg1] > registers[op.arg2]) {
				mFlags |= FLAG_GREATER;
				mFlags &= ~FLAG_EQUALS;
			} else {
				mFlags &= ~(FLAG_GREATER | FLAG_EQUALS);
			}
		} else {
			std::cerr << "Invalid Register(s): 1: " << op.arg1 << "	2: " << op.arg2 << std::endl;
			return -1;
		}
		break;
	case JMP_OP:
		if (op.arg1 < mHeader.text_size) {
			EIP = op.arg1;
		} else {
			std::cerr << "Access Denied: Tried to execute beyond program scope" << std::endl;
			return -2;
		}
		break;
	case SYSCALL_OP:
		ret = Syscall(op.arg1);
		if(ret)
			return ret;
		break;

	case LOOP_OP: //loop
		ECX--;
		if (ECX != 0)
			EIP = op.arg1;
		break;
	}

	return 0;
}

int VM::Syscall(const unsigned char code)
{
	switch(code)
	{
	case 0x00: //exit
		return EDX;
		break;
	case 0x01: //printf
		char * retStr = Utilities::LoadString(this, EAX);
		printf("%s", retStr);
		delete retStr;
		break;
	}
	return 0;
}

int VM::GetOpcodeData(const unsigned int type, const unsigned int val, unsigned int &data)
{
	switch(type)
	{
	case TYPE_Register:
		if(val < NUM_REGISTERS) {
			data = registers[val];
		} else {
			return -1;
		}
		break;
	case TYPE_Address:
		if(val < mHeader.data_size) {
			data = mData[val];
		} else {
			return -2;
		}
		break;
	case TYPE_Constant:
		data = val;
		break;
	}
	return 0;
}
