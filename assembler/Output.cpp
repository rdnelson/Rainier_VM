#include "Output.h"

#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>

#include "Header.h"
#include "Opcode.h"
#include "Registers.h"
#include "Utilities.h"

#ifdef WIN32
	#pragma warning(disable: 4996)
#endif

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

void OutputConstant(char *op, std::string &asmout)
{
	if(isHex(op)) { 
		// TODO: deal with hex, for now return 0
		for(int i = 0; i < 4; i++)
			asmout.push_back(0);
	} else {
		int val = atoi(op);
		for(int i = 0; i < 4; i++)
			asmout.push_back(((char*)&val)[i]);
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