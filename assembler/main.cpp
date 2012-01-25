#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>

#include "Header.h"
#include "Opcode.h"
#include "Registers.h"
#include "Output.h"
#include "Utilities.h"

#define min(a,b) (a > b ? b : a)

#define INVALID_INPUT 1
#define INVALID_OUTPUT 2


#define ERROR(msg) std::cerr << "Error on line " << lineNumber << ": " << msg << std::endl; \
					invalidCode++; \
					continue
#define ERRORLINE(msg,l) std::cerr << "Error on line " << l << ": " << msg << std::endl; \
					invalidCode++; \
					continue



void Usage();
void ParseFile(std::ifstream &fin, std::ofstream &outfile);

int main(int argc, char* argv[])
{
	std::string outfile = "a.rnpe";
	if (argc != 3) {
		Usage();
	} else {
		std::ifstream fin(argv[2]);
		std::ofstream fout(argv[1], std::ios::binary | std::ios::out);
		if(!fin) {
			std::cerr << "Invalid input Filename: " << argv[2] << std::endl;
			return INVALID_INPUT;
		}
		if(!fout) {
			std::cerr << "Invalid output Filename: " << argv[1] << std::endl;
			return INVALID_OUTPUT;
		}
		ParseFile(fin, fout);
		fin.close();
		fout.close();
	}

#ifdef WIN32
	system("pause");
#endif

	return 0;
}

void ParseFile(std::ifstream &fin, std::ofstream &fout)
{
	char line[128] = "";
	char *op;
	int i = 0;
	std::string textout = "";
	std::string dataout = "";
	int retcode = 0;
	int invalidCode = 0;
	int lineNumber = 0;
	std::string tmpTextOut = "";
	std::string tmpDataOut = "";

	
	while(!fin.eof()) {
		tmpTextOut = "";
		fin.getline(line,sizeof(line) - 1);
		lineNumber++;
		retcode = i = 0;

		op = tokenize(line, WHITE);
		if(op == 0)
			continue;

		TO_LOWER(op);
		
		IFOP("mov") {
			tmpTextOut.push_back(MOV_OP);

			char arg1[64];
			memset(arg1, 0, sizeof(arg1));

			op = tokenize(0, WHITE);
			TO_LOWER(op);
			memcpy(arg1, op, min(sizeof(arg1) - 1, strlen(op)));
			int arg1type = GetArgType(op);

			op = tokenize(0, WHITE);
			TO_LOWER(op);
			int arg2type = GetArgType(op);

			if(arg1type == TYPE_Register) { // mov ? -> reg
				if (arg2type == TYPE_Constant) { //? = const
					tmpTextOut.push_back(SUBCODE(SC_REG, SC_CONST));
					OutputRegister(arg1, tmpTextOut);
					if(OutputConstant(op, tmpTextOut)) {
						struct unknown tmp;
						tmp.lineNumber = lineNumber;
						tmp.textpos = tmpTextOut.size() + textout.size();
						unknownLabels[op+1] = tmp;
						OutputConstant((unsigned int)0, tmpTextOut);
					}
				}else if(arg2type == TYPE_Register) {
					tmpTextOut.push_back(SUBCODE(SC_REG, SC_REG));
					OutputSplitRegister(arg1, op, tmpTextOut);
					//OutputRegister(arg1, tmpTextOut);
					//OutputRegister(op, tmpTextOut);
				} else if(arg2type == TYPE_Address) {
					
					tmpTextOut.push_back(SUBCODE(SC_REG, GetAddrType(op)));
					OutputRegister(arg1, tmpTextOut);
					OutputAddress(op, tmpTextOut);

				} else {
					ERROR("Invalid arg2");
				}
			} else if(arg1type == TYPE_Address) {
				if(arg2type == TYPE_Register) {
					tmpTextOut.push_back(SUBCODE(GetAddrType(arg1),SC_REG));
					OutputAddress(op, tmpTextOut);
					OutputRegister(op, tmpTextOut);
				} else {
					ERROR("Invalid arg2");
				}
			} else {
				ERROR("Invalid arg1");
			}
		} else IFOP("nop") {
			tmpTextOut.push_back(NOP_OP);
		} else IFOP("add") {
			tmpTextOut.push_back(ADD_OP);
			invalidCode += OutputOneArg(op, tmpTextOut, textout.size(), lineNumber);
		} else IFOP("sub") {
			tmpTextOut.push_back(SUB_OP);
			invalidCode += OutputOneArg(op, tmpTextOut, textout.size(), lineNumber);
		} else IFOP("mul") {
			tmpTextOut.push_back(MUL_OP);
			invalidCode += OutputOneArg(op, tmpTextOut, textout.size(), lineNumber);
		} else IFOP("div") {
			tmpTextOut.push_back(DIV_OP);
			invalidCode += OutputOneArg(op, tmpTextOut, textout.size(), lineNumber);
		} else IFOP("shr") {
			tmpTextOut.push_back(SHR_OP);
			invalidCode += OutputOneArg(op, tmpTextOut, textout.size(), lineNumber);
		} else IFOP("shl") {
			tmpTextOut.push_back(SHL_OP);
			invalidCode += OutputOneArg(op, tmpTextOut, textout.size(), lineNumber);
		} else IFOP("push") {
			tmpTextOut.push_back(PUSH_OP);
			invalidCode += OutputOneArg(op, tmpTextOut, textout.size(), lineNumber);
		} else IFOP("pop") {
			tmpTextOut.push_back(POP_OP);
			op = tokenize(0, WHITE);
			switch(GetArgType(op))
			{
			case TYPE_Register:
				tmpTextOut.push_back(SC_REG);
				OutputRegister(op, tmpTextOut);
				break;
			case TYPE_Address:
				tmpTextOut.push_back(GetAddrType(op));
				OutputAddress(op, tmpTextOut);
				break;
			default:
				ERROR("Can only pop to an address or register.");
			}
		} else IFOP("jmp") {
			tmpTextOut.push_back(JMP_OP);
			invalidCode += OutputOneArg(op, tmpTextOut, textout.size(), lineNumber);
		} else IFOP("test") {
			tmpTextOut.push_back(TEST_OP);
			op = tokenize(0, WHITE);
			TO_LOWER(op);
			char arg1[64];
			memset(arg1, 0, sizeof(arg1));

			op = tokenize(0, WHITE);
			TO_LOWER(op);
			memcpy(arg1, op, min(sizeof(arg1) - 1, strlen(op)));
			int arg1type = GetArgType(op);

			op = tokenize(0, WHITE);
			TO_LOWER(op);
			int arg2type = GetArgType(op);
			if(arg1type == TYPE_Register) { // mov ? -> reg
				if (arg2type == TYPE_Constant) { //? = const
					tmpTextOut.push_back(SUBCODE(SC_REG, SC_CONST));
					OutputRegister(arg1, tmpTextOut);
					if(OutputConstant(op, tmpTextOut)) {
						struct unknown tmp;
						tmp.lineNumber = lineNumber;
						tmp.textpos = tmpTextOut.size() + textout.size();
						unknownLabels[op+1] = tmp;
						OutputConstant((unsigned int)0, tmpTextOut);
					}
				}else if(arg2type == TYPE_Register) {
					tmpTextOut.push_back(SUBCODE(SC_REG, SC_REG));
					OutputSplitRegister(arg1, op, tmpTextOut);
					//OutputRegister(arg1, tmpTextOut);
					//OutputRegister(op, tmpTextOut);
				} else if(arg2type == TYPE_Address) {
					tmpTextOut.push_back(SUBCODE(SC_REG, GetAddrType(op)));
					OutputRegister(arg1, tmpTextOut);
					OutputAddress(op, tmpTextOut);
				} else {
					ERROR("Invalid arg2");
				}
			} else {
				ERROR("Invalid arg1: Must be register");
			}
		} else IFOP("and") {
			tmpTextOut.push_back(AND_OP);
			invalidCode += OutputOneArg(op, tmpTextOut, textout.size(), lineNumber);
		} else IFOP("or") {
			tmpTextOut.push_back(OR_OP);
			invalidCode += OutputOneArg(op, tmpTextOut, textout.size(), lineNumber);
		} else IFOP("xor") {
			tmpTextOut.push_back(XOR_OP);
			invalidCode += OutputOneArg(op, tmpTextOut, textout.size(), lineNumber);
		} else IFOP("not") {
			tmpTextOut.push_back(NOT_OP);
		} else IFOP("loop") {
			tmpTextOut.push_back(LOOP_OP);
			invalidCode += OutputOneArg(op, tmpTextOut, textout.size(), lineNumber);
		} else IFOP("je") {
			tmpTextOut.push_back(JE_OP);
			invalidCode += OutputOneArg(op, tmpTextOut, textout.size(), lineNumber);
		} else IFOP("jne") {
			tmpTextOut.push_back(JNE_OP);
			invalidCode += OutputOneArg(op, tmpTextOut, textout.size(), lineNumber);
		} else IFOP("jgt") {
			tmpTextOut.push_back(JGT_OP);
			invalidCode += OutputOneArg(op, tmpTextOut, textout.size(), lineNumber);
		} else IFOP("jge") {
			tmpTextOut.push_back(JGE_OP);
			invalidCode += OutputOneArg(op, tmpTextOut, textout.size(), lineNumber);
		} else IFOP("jlt") {
			tmpTextOut.push_back(JLT_OP);
			invalidCode += OutputOneArg(op, tmpTextOut, textout.size(), lineNumber);
		} else IFOP("jle") {
			tmpTextOut.push_back(JLE_OP);
			invalidCode += OutputOneArg(op, tmpTextOut, textout.size(), lineNumber);
		} else IFOP("sys") {
			tmpTextOut.push_back((char)SYSCALL_OP);
		} else IFOP("ds") { //string data
			op = tokenize(0, WHITE, '"', "\"");
			TO_LOWER(op);
			if(( Data.find(op)) == Data.end()) {
				Data[op] = dataout.size();
				op = tokenize(0, WHITE, '"', "\"");
				op++;
				
				int len = strlen(op);
				for(int j = 0; j < 4; j++)
					dataout.push_back(((char*)&len)[j]);
				dataout.append(op);
			}
		} else if(op[0] == ':') { //label
			if(strlen(op+1) > 0) {
				if(Labels.find(op+1) == Labels.end()) {
					Labels[op+1] = tmpTextOut.size() + textout.size();
				} else {
					ERROR("Label '" << op + 1 << "' already defined");
				}
			} else {
				ERROR("Labels must be named");
			}
		}

		textout += tmpTextOut;
	} 

	for(Label i = unknownLabels.begin(); i != unknownLabels.end(); i++) {
		if(Labels.find(i->first) != Labels.end()) {
			int val = Labels[i->first];
			for(int j = 0; j < 4; j++)
				textout[i->second.textpos + j] = ((char*)&val)[j];
		} else {
			ERRORLINE("Undefined label '" << i->first << "'", i->second.lineNumber);
		}

	}
	

	if(invalidCode) {
		std::cerr << "There were " << invalidCode << " errors during assembly" << std::endl;
		return;
	}

	//Output the final executable
	RNPE_Header header;
	memset(&header, 0, sizeof(header));
	
	SET_HEADER_MAGIC(header);
	
	header.text_pos = sizeof(header);
	header.text_size = textout.size();
	header.data_pos = textout.size() + header.text_pos;
	header.data_size = dataout.size();
	header.size = header.text_size + header.data_size + sizeof(header);
	header.entry_pos = 0; //TODO dynamic entry point, probably happen along with labels
	fout.write((char*)&header, sizeof(header));
	fout.write(textout.c_str(), textout.size());
	fout.write(dataout.c_str(), dataout.size());
}
	
void Usage()
{
	std::cout << "rnpe-asm <output> <input>" << std::endl;
}
