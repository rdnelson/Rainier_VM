#include "Output.h"

#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include <cstdlib>

#include "common/Header.h"
#include "common/Opcode.h"
#include "common/Registers.h"
#include "Utilities.h"

#ifdef WIN32
	#pragma warning(disable: 4996)
#endif

std::map<std::string, unsigned int> Labels;
std::map<std::string, struct unknown> unknownLabels;
std::map<std::string, unsigned int> Data;

void OutputRegister(char *op, std::string &asmout)
{
	for(int i = 0; i < NUM_REGISTERS; i++) {
		IFOP(STR_Registers[i])
			asmout.push_back(i);
	}
}

void OutputSplitRegister(char *op1, char *op2, std::string &asmout)
{
	int reg[2];
	char *op;
	for(int j = 0; j < 2; j++){
		if (j == 0)
			op = op1;
		else
			op = op2;

		for(int i = 0; i < NUM_REGISTERS; i++) {
			IFOP(STR_Registers[i])
				reg[j] = i;
		}
	}
	asmout.push_back((char)(((reg[0] << 4) & 0xF0) | (reg[1] & 0x0F) ));
}

int OutputConstant(char *op, std::string &asmout)
{
	if(op && strlen(op) >= 2 && op[0] == ':') {
		if(Labels.find(op+1) != Labels.end())
			OutputConstant(Labels[op+1], asmout);
		else 
			return 1;
		return 0;
	} else if(op && strlen(op) >= 2 && op[0] == '@'){
		if(Data.find(op+1) != Data.end())
			OutputConstant(Data[op+1], asmout);
		else 
			return 1;
		return 0;
	}
	if(isHex(op)) { 
		// TODO: deal with hex, for now return 0
		for(int i = 0; i < 4; i++)
			asmout.push_back(0);
	} else {
		int val = atoi(op);
		for(int i = 0; i < 4; i++)
			asmout.push_back(((char*)&val)[i]);
	}
	return 0;
}

void OutputConstant(unsigned int val, std::string &asmout)
{
	for(int i = 0; i < 4; i++)
			asmout.push_back(((char*)&val)[i]);
}

void OutputId(char *op, std::string &asmout)
{
	if(isHex(op)) { 
		// TODO: deal with hex, for now return 0
		for(int i = 0; i < 1; i++)
			asmout.push_back(0);
	} else {
		asmout.push_back((char)(atoi(op) & 0xFF));
	}
}

void OutputAddress(char *op, std::string &asmout)
{
	char *tok;
	switch(GetAddrType(op))
	{
	case SC_CONST_ADD:
	case SC_CONST_M_EAX:
	case SC_CONST_P_EAX:
	case SC_EBX_P_CONST:
	case SC_EBX_M_CONST:
		tok = strtok(op+1, WHITE);
		while(tok) {
			if (GetArgType(tok) == TYPE_Constant) {
				OutputConstant(tok, asmout);
			}
			tok = strtok(0, WHITE);
		}
		break;
	default:
		break; //no need to output anything, the subcode takes care of that for us.
	}
}

int OutputOneArg(char *op, std::string &asmout, unsigned int textpos, unsigned int lineNumber)
{
	op = tokenize(0, WHITE);
	TO_LOWER(op);
	int argtype = GetArgType(op);
	std::cerr << "Opcode: " << op << " only argument is type: " << argtype << std::endl;
	switch(argtype)
	{
	case TYPE_Constant:
		asmout.push_back(SC_CONST << 4);
		if(OutputConstant(op, asmout)) {
			struct unknown tmp;
			tmp.lineNumber = lineNumber;
			tmp.textpos = asmout.size() + textpos;
			unknownLabels[op+1] = tmp;
			OutputConstant((unsigned int)0, asmout);
		}
		break;
	case TYPE_Register:
		asmout.push_back(SC_REG << 4);
		OutputRegister(op, asmout);
		break;
	case TYPE_Address:
		asmout.push_back(GetAddrType(op) << 4);
		OutputAddress(op, asmout);
		break;
	default:
		return 1;
	}
	return 0;
}
