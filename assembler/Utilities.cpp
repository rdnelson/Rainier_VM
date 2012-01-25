#include "Utilities.h"

#include <cstring>
#include <cctype>

#include "Registers.h"
#include "Opcode.h"

#ifdef WIN32
	#pragma warning(disable: 4996)
#endif

char* tokenize(char * str, const char* delim, char bracket, char* closebracket) //will return [.... with no end bracket for address type strings
{
	static char* strCopy = 0;

	char *token = 0;

	if(!str) {
		if(!strCopy)
			return 0;
		else
			str = strCopy;
	}

	str += strspn(str, delim);
	if(*str == 0) {
		strCopy = 0;
		return 0;
	}

	token = str;
	if (*str == bracket) {
		str = strpbrk(token+1, closebracket);
	} else {
		str = strpbrk(token, delim);
	}
	if(!str) {
		strCopy = 0;
	} else {
		*str = 0;
		strCopy = str + 1;
	}
	return token;
}

int isHex(char *op)
{
	int len = strlen(op);
	if(len >= 3) { // must have 0x and at least one digit
		if(op[0] == '0' && op[1] == 'x') {
			for (int i = 2; i < len; i++) { // check that all digits are valid
				if (!isxdigit(op[i]))
					return 0;
			}
			return 1;
		}
	}
	return 0;
}

int GetArgType(char *op)
{
	int len = strlen(op);

	//check if register
	for(int i = 0; i < NUM_REGISTERS; i++) {
		IFOP(STR_Registers[i])
			return TYPE_Register;
	}

	if(!strcmp(op, "+") | !strcmp(op, "-"))
		return TYPE_Operator;

	//check if address
	if(len >=2) { // must have [ and something after it
		if(op[0] == '[') { //check bracket is in place (tokenize only provides the first one)
			op++;
			if (GetAddrType(op) == -1)
				return TYPE_None;
			else
				return TYPE_Address;
		}
	}

	if(len >= 2)
		if(op[0] == ':' || op[0] == '@')
			return TYPE_Constant;


	//check if constant
	if(isHex(op))
		return TYPE_Constant;

	bool valid = true;
	for(int i = 0; i < len; i++) {
		if(!isdigit(op[i])) {
			valid = false;
			break;
		}
	}
	if(valid && strlen(op) > 0)
		return TYPE_Constant;

	return TYPE_None;
}

int GetAddrType(char* addr)
{
	char * strCopy = new char[strlen(addr)+1];
	char * op = strCopy;
	memcpy(strCopy, addr, strlen(addr)+1);
	int arg1, arg1type, arg2, arg2type, arg3, arg3type;
	if(strlen(op) == 0) {
		delete strCopy;
		return -1;
	}
	if (op[0] == ':' || op[0] == '@')
		return SC_CONST_ADD;

	if (op[0] == '[')
		op++;
	char *tok = strtok(op, WHITE);
	arg1type = GetArgType(tok);

	//if the first argument is a register but not ebx, it's invalid
	if(arg1type == TYPE_Register) {
		arg1 = GetRegister(tok);
		if(arg1 != REG_EBX) {
			delete strCopy;
			return -1;
		}

	} else if(arg1type == TYPE_Constant) {
		//arg1 = atoi(tok);
	} else {
		delete strCopy;
		return -1;
	}

	tok = strtok(0, WHITE);		

	//if there's only one argument it's either constant, ebx or invalid
	if(tok == 0) {
		delete strCopy;
		if (arg1type == TYPE_Constant)
			return SC_CONST_ADD;
		else if (arg1type == TYPE_Register && arg1 == REG_EBX)
			return SC_EBX;
		else 
			return -1;
	}

	//if the second argument isn't an operator, it's invalid
	arg2type = GetArgType(tok);
	if(arg2type != TYPE_Operator) {
		delete strCopy;
		return -1;
	}

	//makes the conditionals below easier;
	arg2 = tok[0];

	tok = strtok(0, WHITE);

	//if there are only two arguments, it's invalid
	if(!tok) {
		delete strCopy;
		return -1;
	}

	//if the third argument is a register but not eax, it's invalid
	arg3type = GetArgType(tok);
	if(arg3type == TYPE_Register) {
		arg3 = GetRegister(tok);
		if(arg3 != REG_EAX) {
			delete strCopy;
			return -1;
		}
	} else if(arg3type == TYPE_Constant) {
		//arg3 = atoi(tok);
	} else {
		delete strCopy;
		return -1;
	}

	//ensure that there are at max three tokens
	if (strtok(0, WHITE)) {
		delete strCopy;
		return -1;
	}

	//register identities have already been checked
	if (arg1type == TYPE_Constant) {
		delete strCopy;
		if (arg3type == TYPE_Register) 
			return arg2 == '+' ? SC_CONST_P_EAX : SC_CONST_M_EAX;
		else
			return -1;
	}

	if (arg1type == TYPE_Register) {
		delete strCopy;
		if (arg3type == TYPE_Register)
			return arg2 == '+' ? SC_EBX_P_EAX : SC_EBX_M_EAX;
		else if (arg3type == TYPE_Constant)
			return arg2 == '+' ? SC_EBX_P_CONST : SC_EBX_M_CONST;
		else
			return -1;
	}

	delete strCopy; 

	return -1;
}

int GetRegister(char *op)
{
	for(int i = 0; i < NUM_REGISTERS; i++) {
		IFOP(STR_Registers[i])
			return i;
	}
	return -1; //should never hit this.
}