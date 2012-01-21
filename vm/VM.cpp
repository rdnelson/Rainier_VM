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
	std::cout << "VM created. mReady: " << mReady << std::endl;
}

VM::~VM() {
	// TODO Auto-generated destructor stub
}

void VM::run()
{
	std::cout << "Running" << std::endl;
	if (!mReady)
		return;
	std::cout << "Still running" << std::endl;
	std::ifstream fin(mOpts->GetExe().c_str());
	if(!fin) {
		mReady = false;
		return;
	}
	std::cout << "Executable loaded" << std::endl;

	//populate header
	fin.read((char*)&mHeader, sizeof(RNPE_Header));

	std::cout << "Header read in" << std::endl;

	//ensure sizes all match up
	if (mHeader.text_size + mHeader.data_size + sizeof(RNPE_Header) != mHeader.size) {
		mReady = false;
		fin.close();
		std::cout << "Header sizes don't match up" << std::endl;
		return;
	}

	//create data and text buffers
	mData = new char[mHeader.data_size];
	mText = new char[mHeader.text_size];

	std::cout << "Created mData and mText" << std::endl;

	//load in data and text
	fin.seekg(mHeader.data_pos);
	fin.read(mData, mHeader.data_size);
	unsigned int read_count = fin.gcount();
	if( read_count != mHeader.data_size ) {
		mReady = false;
		std::cout << "Invalid read size: data" << std::endl;
		fin.close();
		return;
	}
	//if( mHeader.text_pos < mHeader.data_pos 

	fin.seekg(mHeader.text_pos);
	fin.read(mText, mHeader.text_size);
	read_count = fin.gcount();
	std::cout << "Characters in text: " << read_count << std::endl;
	if( read_count != mHeader.text_size ) {
		mReady = false;
		std::cout << "Invalid read size: text" << std::endl;
		fin.close();
		return;
	}

	fin.close();

	Execute();

}

void VM::Execute()
{
	std::cout << "Beginning execution" << std::endl;
	if( mHeader.entry_pos > mHeader.text_size )
		return;

	EIP = mHeader.entry_pos;
	Opcode op;
	int retVal;
	do
	{
		std::cout << "Reading opcode" << std::endl;
		op = ReadOpcode();
		retVal = ExecuteOpcode(op);
	} while( !retVal );

}

Opcode VM::ReadOpcode()
{
	Opcode tmp;
	int v = EIP;
	tmp.Opcode = mText[v];
	EIP++;
	std::cout << "Opcode:" << (int)tmp.Opcode << std::endl;

	switch( tmp.Opcode )
	{
	case 0x00: //nop
	case 0x09: //pop
	case 0x0F: //not
		break;
	case 0x01: //mov add -> reg
	case 0x0B: //test
		tmp.arg1type = TYPE_Register;
		//tmp.arg1 = (unsigned int)mText[eip];
		//eip++;
		load_argn(tmp.arg1, 1);

		tmp.arg2type = TYPE_Address;
		//memcpy((char*)&tmp.arg2, &mText[eip], ADDR_SIZE);
		load_arg(tmp.arg2);
		//eip += ADDR_SIZE;
		break;
	case 0x02: //add (1 arg)
	case 0x03: //sub (1 arg)
	case 0x04: //mul (1 arg)
	case 0x05: //div (1 arg)
	case 0x06: //shr (1 arg)
	case 0x07: //shl (1 arg)
	case 0x08: //push (1 arg)
	case 0x0A: //jmp
	case 0x0C: //and
	case 0x0D: //or
	case 0x0E: //xor
	case 0x10: //loop
		tmp.arg1type = TYPE_Address;
		load_arg(tmp.arg1);
		tmp.arg2type = TYPE_None;
		break;
	case 0xFF: //syscall
		tmp.arg1type = TYPE_Id;
		load_argn(tmp.arg1, 1);
		tmp.arg2type = TYPE_None;
	}
	return tmp;
}

int VM::ExecuteOpcode(const Opcode &op)
{
	int ret = 0;
	std::cout << "Executing opcode: " << (int) op.Opcode << std::endl;
	switch(op.Opcode)
	{
	case 0x01: //mov
		if (op.arg1 < NUM_REGISTERS) {
			registers[op.arg1] = op.arg2;
		} else {
			std::cout << "Invalid Register" << std::endl;
			return -1;
		}
		break;
	case 0x0B: //test reg <> addr
		{
			unsigned int val1, val2;
			switch(op.arg1type)
			{
			case TYPE_Register:
				if(op.arg1 < NUM_REGISTERS) {
					val1 = registers[op.arg1];
				} else {
					return -1;
				}
				break;
			case TYPE_Address:
				val1 = mData[op.arg1];
				break;
			case TYPE_Constant:
				val1 = op.arg1;
				break;
			}
			GetOpcodeData(op.arg2type, op.arg2, val2);

		}
		break;
	case 0xFF:
		ret = Syscall(op.arg1);
		if(ret)
			return ret;
		break;

	case 0x10: //loop
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
