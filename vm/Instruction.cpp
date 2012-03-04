#include "Instruction.h"
#include "VM.h"
#include "Instructions.h"
#include "common/Opcode.h"


#include <cstring>

Instruction* Instruction::ReadInstruction(unsigned int eip)
{
	char* eipMem = VM_INSTANCE()->GetMemory(eip);

	if(!eipMem)
		return 0;

	char Opcode = *eipMem;

	VM_INSTANCE()->GetLogger() << "Opcode is: 0x" << std::hex << Opcode << std::dec << std::endl;
	switch(Opcode){
	case NOP_OP:
		return new Nop(eipMem);
		break;
	case MOV_OP:
		return new Mov(eipMem);
		break;
	case ADD_OP:
		return new Add(eipMem);
		break;
	case SUB_OP:
		return new Sub(eipMem);
		break;
	case MUL_OP:
		return new Mul(eipMem);
		break;
	case DIV_OP:
		return new Div(eipMem);
		break;
	case SHR_OP:
		return new Shr(eipMem);
		break;
	case SHL_OP:
		return new Shl(eipMem);
		break;
	case PUSH_OP:
		return new Push(eipMem);
		break;
	case POP_OP:
		return new Pop(eipMem);
		break;
	case JMP_OP:
		return new Jmp(eipMem);
		break;
	case TEST_OP:
		return new Test(eipMem);
		break;
	case AND_OP:
		return new And(eipMem);
		break;
	case OR_OP:
		return new Or(eipMem);
		break;
	case XOR_OP:
		return new Xor(eipMem);
		break;
	case NOT_OP:
		return new Not(eipMem);
		break;
	case LOOP_OP:
		return new Loop(eipMem);
		break;
	case JE_OP:
		return new Je(eipMem);
		break;
	case JNE_OP:
		return new Jne(eipMem);
		break;
	case JGT_OP:
		return new Jgt(eipMem);
		break;
	case JGE_OP:
		return new Jge(eipMem);
		break;
	case JLT_OP:
		return new Jlt(eipMem);
		break;
	case JLE_OP:
		return new Jle(eipMem);
		break;
	case SYSCALL_OP:
		return new Sys(eipMem);
		break;
	case MOVB_OP:
		return new Movb(eipMem);
		break;
	case CALL_OP:
		return new Call(eipMem);
		break;
	case RET_OP:
		return new Ret(eipMem);
		break;
	}
	return 0;
}

Instruction::Instruction() : mEipOffset(0), mValid(true), mErr(0)
{
}

unsigned long Instruction::LoadArgs(unsigned int numArgs, char* eip)
{
	char argCodes;

	unsigned long tmpEip = (unsigned long)eip;

	eip++; //eliminate opcode

	switch(numArgs)
	{
	case 0:
		//Nothing left to read
		break;
	case 1:
	case 2:
		argCodes = *eip;
		eip++;
		for(unsigned int i = 0; i < numArgs; i++) {
			unsigned int tmp = LoadOneArg(i, argCodes, eip);
			VM_INSTANCE()->GetLogger() << "Argument " << i << " has size " << tmp << std::endl;
			eip += tmp;
		}
		break;
	}
	return (unsigned long)eip - tmpEip;

}

unsigned int Instruction::LoadOneArg(unsigned int argNum, char argCodes, char* eip)
{
	unsigned int length = 0, tmpArg = 0;
	argNum = (argNum == 0 ? 0 : 1);
	subcode[argNum] = (argNum == 0 ? ((argCodes >> 4) & 0x0F) : (argCodes & 0x0F));
	VM_INSTANCE()->GetLogger() << "Argument " << argNum << " subcode is: " << std::hex << subcode[argNum] << std::dec << std::endl;
	switch(subcode[argNum])
	{
	case SC_NONE:
	case SC_EBX:
		arguments[argNum] = VM_INSTANCE()->GetRegister(REG_EBX);
		break;
	case SC_EBX_P_EAX:
		arguments[argNum] = VM_INSTANCE()->GetRegister(REG_EBX) + VM_INSTANCE()->GetRegister(REG_EAX);
		break;
	case SC_EBX_M_EAX:
		arguments[argNum] = VM_INSTANCE()->GetRegister(REG_EBX) - VM_INSTANCE()->GetRegister(REG_EAX);
		break;
	case SC_REG:
		if(argCodes != (SC_REG << 4 | SC_REG)) {
			length = 1;
			memcpy(&tmpArg, eip, 1);
			arguments[argNum] = tmpArg;
		} else {
			length = argNum;
			arguments[argNum] = (argNum ? (*eip & 0xF) : ((*eip >> 4) & 0xF));
		}
		break;
	case SC_CONST:
		length = 4;
		memcpy(&tmpArg, eip, 4);
		arguments[argNum] = tmpArg;
		break;

	case SC_CONST_ADD:
		length = 4;
		memcpy(&tmpArg, eip, 4);
		arguments[argNum] = tmpArg;
		break;

	case SC_CONST_P_EAX:
		length = 4;
		memcpy(&tmpArg, eip, 4);
		arguments[argNum] = tmpArg + VM_INSTANCE()->GetRegister(REG_EAX);
		break;

	case SC_CONST_M_EAX:
		length = 4;
		memcpy(&tmpArg, eip, 4);
		arguments[argNum] = tmpArg - VM_INSTANCE()->GetRegister(REG_EAX);
		break;

	case SC_EBX_P_CONST:
		length = 4;
		memcpy(&tmpArg, eip, 4);
		arguments[argNum] = VM_INSTANCE()->GetRegister(REG_EBX) + tmpArg;
		break;

	case SC_EBX_M_CONST:
		length = 4;
		memcpy(&tmpArg, eip, 4);
		arguments[argNum] = VM_INSTANCE()->GetRegister(REG_EBX) - tmpArg;
		break;

	}

	return length;
}

void Instruction::ResolveValue(unsigned int arg)
{
	//ensure valid bounds
	if(arg < 0 || arg > 1)
		return;

	char* src = VM_INSTANCE()->GetMemory(arguments[arg]);

	switch(subcode[arg]) {
	case SC_REG:
		arguments[arg] = VM_INSTANCE()->GetRegister(arguments[arg]);
		break;
	case SC_EBX:
	case SC_CONST_ADD:
	case SC_CONST_P_EAX:
	case SC_CONST_M_EAX:
	case SC_EBX_P_CONST:
	case SC_EBX_M_CONST:
	case SC_EBX_P_EAX:
	case SC_EBX_M_EAX:
		if(src)
			memcpy(&arguments[arg], src, 4);
		break;

	}
}
