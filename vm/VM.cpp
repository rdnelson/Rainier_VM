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
#include "common/Header.h"

VM::VM(Options *opts) : mOpts(opts) {
	// TODO Auto-generated constructor stub
	mReady = opts->IsValid();

	mCerrBackup = std::cerr.rdbuf();

	if (!opts->IsVerbose()){
		std::cerr.rdbuf(mErr.rdbuf());
	}
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
	std::ifstream fin(mOpts->GetExe().c_str(), std::ios::binary);
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
	unsigned int read_count = (int)fin.gcount();
	if( read_count != mHeader.data_size ) {
		mReady = false;
		std::cerr << "Invalid read size: data" << std::endl;
		fin.close();
		return;
	}
	//if( mHeader.text_pos < mHeader.data_pos 

	fin.seekg(mHeader.text_pos);
	fin.read(mText, mHeader.text_size);
	read_count = (int)fin.gcount();
	std::cerr << "Characters in text: " << read_count << std::endl;
	if( read_count != mHeader.text_size ) {
		mReady = false;
		std::cerr << "Invalid read size: text" << std::endl;
		fin.close();
		return;
	}

	fin.close();

	Execute();

	std::cerr.rdbuf(mCerrBackup);

}

void VM::Execute()
{
	std::cerr << "Beginning execution" << std::endl;
	if( mHeader.entry_pos > mHeader.text_size )
		return;

	EIP = mHeader.entry_pos;
	Opcode op;
	int retVal, retCode;
	do
	{
		std::cerr << "----------------------------------------------------" << std::endl;
		std::cerr << "Reading opcode" << std::endl;
		op = ReadOpcode();
		retVal = ExecuteOpcode(op, &retCode);
	} while( !retVal );

}

Opcode VM::ReadOpcode()
{
	Opcode tmp;
	tmp.isValid = 1;

	load_byte(tmp.opcode);
	std::cerr << "Opcode:" << (int)tmp.opcode << std::endl;

	Utilities::LoadOpcodeArgs(&tmp, mText, registers);
	return tmp;
}

int VM::ExecuteOpcode (Opcode &op, int * retCode)
{
	if (!op.isValid)
		return -3;
	//int ret = 0;
	std::cerr << "Executing opcode: " << (int) op.opcode << std::endl;
	op.printop();
	switch(op.opcode)
	{
	case MOV_OP: //mov
		std::cerr << "Executing Mov" << std::endl;
		ResolveOpcodeArg(op, 1);
		switch(op.argtype[0])
		{
		case TYPE_Register:
			std::cerr << "Assigning " << STR_Registers[op.args[0]] << " = " << op.args[1] << std::endl;
			op.printop();
			registers[op.args[0]] = op.args[1];
			break;
		case TYPE_Address:
			mData[op.args[0]] = op.args[1];
			break;
		case TYPE_Def_Address:
			ResolveOpcodeArg(op, 0);
			mData[op.args[0]] = op.args[1];
			break;
		default:
			std::cerr << "Invalid Mov operation" << std::endl;
		}
		break;
	case PUSH_OP:
		ResolveOpcodeArg(op,0);
		mStack.push(op.args[0]);
		break;
	case POP_OP:
		switch(op.argtype[0])
		{
		case TYPE_Register:
			registers[op.args[0]] = mStack.top();
			break;
		case TYPE_Address:
			mData[op.args[0]] = mStack.top();
			break;
		case TYPE_Def_Address:
			ResolveOpcodeArg(op, 0);
			mData[op.args[0]] = mStack.top();
			break;
		}
		mStack.pop();
		break;
	case AND_OP:
		ResolveOpcodeArg(op,0);
		EAX &= op.args[0];
		break;
	case OR_OP:
		ResolveOpcodeArg(op,0);
		EAX |= op.args[0];
		break;
	case XOR_OP:
		ResolveOpcodeArg(op,0);
		EAX ^= op.args[0];
		break;
	case NOT_OP:
		EAX = ~EAX;
		break;
	case TEST_OP:
		ResolveOpcodeArg(op,0);
		ResolveOpcodeArg(op,1);

		if (op.args[0] < op.args[1] ) {
			mFlags &= ~FLAG_EQUALS;
			mFlags &= ~FLAG_GREATER;
		} else if(op.args[0] > op.args[1] ) {
			mFlags &= ~FLAG_EQUALS;
			mFlags |= FLAG_GREATER;
		} else {
			mFlags |= FLAG_EQUALS;
			mFlags &= ~FLAG_GREATER;
		}
		break;
	case JMP_OP:
		ResolveOpcodeArg(op, 0);
		if (op.args[0] < mHeader.text_size) {
			EIP = op.args[0];
		} else {
			std::cerr << "Access Denied: Tried to execute beyond program scope" << std::endl;
			return -2;
		}
		break;
	case JE_OP:
		ResolveOpcodeArg(op, 0);
		if (op.args[0] < mHeader.text_size) {
			if ((mFlags & FLAG_EQUALS))
				EIP = op.args[0];
		} else {
			std::cerr << "Access Denied: Tried to execute beyond program scope" << std::endl;
			return -2;
		}
		break;
	case JNE_OP:
		ResolveOpcodeArg(op, 0);
		if (op.args[0] < mHeader.text_size) {
			if (!(mFlags & FLAG_EQUALS))
				EIP = op.args[0];
		} else {
			std::cerr << "Access Denied: Tried to execute beyond program scope" << std::endl;
			return -2;
		}
		break;
	case JGT_OP:
		ResolveOpcodeArg(op, 0);
		if(op.args[0] < mHeader.text_size) {
			if (mFlags & FLAG_GREATER)
				EIP = op.args[0];
		} else {
			std::cerr << "Access Denied: Tried to execute beyond program scope" << std::endl;
			return -2;
		}
		break;
	case JLT_OP:
		ResolveOpcodeArg(op, 0);
		if(op.args[0] < mHeader.text_size) {
			if (!(mFlags & FLAG_GREATER))
				EIP = op.args[0];
		} else {
			std::cerr << "Access Denied: Tried to execute beyond program scope" << std::endl;
			return -2;
		}
		break;
	case JGE_OP:
		ResolveOpcodeArg(op, 0);
		if(op.args[0] < mHeader.text_size) {
			if (mFlags & (FLAG_GREATER | FLAG_EQUALS))
				EIP = op.args[0];
		} else {
			std::cerr << "Access Denied: Tried to execute beyond program scope" << std::endl;
			return -2;
		}
		break;
	case JLE_OP:
		ResolveOpcodeArg(op, 0);
		if(op.args[0] < mHeader.text_size) {
			if (!(mFlags & (FLAG_GREATER | FLAG_EQUALS)))
				EIP = op.args[0];
		} else {
			std::cerr << "Access Denied: Tried to execute beyond program scope" << std::endl;
			return -2;
		}
		break;
	case SYSCALL_OP:
		if(retCode)
			*retCode = Syscall();
		return -1;
		break;

	case LOOP_OP: //loop
		ECX--;
		ResolveOpcodeArg(op, 0);
		op.printop();
		if (ECX != 0)
			EIP = op.args[0];
		break;
	}

	return 0;
}

int VM::Syscall()
{
	std::cerr << "Syscall made: executing command 0x" << std::hex << (int)EAX << std::dec << std::endl;
	switch(EAX)
	{
	case 0x00: //exit
		return EDX;
		break;
	case 0x01: //printf
		char * retStr = Utilities::LoadString(this, EDX);
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

void VM::ResolveOpcodeArg(Opcode &op, unsigned int arg)
{
	if (arg >= 2)
		return;

	switch(SUBCODE_N(arg, op.subcode))
	{
	case SC_REG:
		op.args[arg] = registers[op.args[arg]];
		break;
	case SC_CONST:
		break;
	case SC_CONST_ADD:
		memcpy(&op.args[arg], &mData[op.args[arg]], sizeof(op.args[arg]));
		break;
	case SC_EBX:
		op.args[arg] = EBX;
		break;
	case SC_EBX_P_EAX:
		op.args[arg] = EBX + EAX;
		break;
	case SC_EBX_M_EAX:
		op.args[arg] = EBX - EAX;
		break;
	case SC_CONST_P_EAX:
		op.args[arg] = op.args[arg] + EAX;
		break;
	case SC_CONST_M_EAX:
		op.args[arg] = op.args[arg] - EAX;
		break;
	case SC_EBX_P_CONST:
		op.args[arg] = EBX + op.args[arg];
		break;
	case SC_EBX_M_CONST:
		op.args[arg] = EBX - op.args[arg];
		break;
	default:
		std::cerr << "Invalid subcode: " << SUBCODE_N(arg, op.subcode) << std::endl;
	}
}
