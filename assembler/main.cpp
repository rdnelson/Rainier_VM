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

#define ERROR(msg) std::cerr << "Error on line:" << lineNumber << " " << msg << std::endl; \
					invalidCode++; \
					continue



void Usage();
void ParseFile(std::ifstream &fin, std::ofstream &outfile);

int main(int argc, char* argv[])
{
	std::string outfile = "a.rnpe";
	if (argc != 3) {
		Usage();
	} else
	{
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
		while (line[i]) {
			line[i] = tolower(line[i]);
			i++;
		}

		op = tokenize(line, WHITE);
		
		IFOP("mov") {
			tmpTextOut.push_back(MOV_OP);

			char arg1[64];
			memset(arg1, 0, sizeof(arg1));

			op = tokenize(0, WHITE);
			memcpy(arg1, op, min(sizeof(arg1) - 1, strlen(op)));
			int arg1type = GetArgType(op);

			op = tokenize(0, WHITE);
			int arg2type = GetArgType(op);

			if(arg1type == TYPE_Register) { // mov ? -> reg
				if (arg2type == TYPE_Constant) { //? = const
					tmpTextOut.push_back(SUBCODE(SC_REG, SC_CONST));
					OutputRegister(arg1, tmpTextOut);
					OutputConstant(op, tmpTextOut);
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
					OutputAddress(arg1, tmpTextOut);
					OutputRegister(op, tmpTextOut);
				} else {
					ERROR("Invalid arg2");
				}
			} else {
				ERROR("Invalid arg1");
			}
		}


		textout += tmpTextOut;
	}

	if(invalidCode)
		return;

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