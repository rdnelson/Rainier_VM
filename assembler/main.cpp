#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>

#include "common/Header.h"
#include "common/Opcode.h"
#include "common/Registers.h"
#include "Instruction.h"

#define min(a,b) (a > b ? b : a)

#define INVALID_INPUT 1
#define INVALID_OUTPUT 2


#define ERROR(msg) std::cerr << "Error on line " << lineNumber << ": " << msg << std::endl; \
					invalidCode++; \
					continue
#define ERRORLINE(msg,l) std::cerr << "Error on line " << l << ": " << msg << std::endl; \
					invalidCode++; \
					continue
//"" for nano syntax highlighting


void Usage();
void ParseFile(std::ifstream &fin, std::ofstream &outfile, bool raw);

int main(int argc, char* argv[])
{
	bool raw = false;
	if (argc != 3 && argc != 4) {
		Usage();
		return -1;
	} else if(argc == 4) {
		if(!strcmp(argv[3], "-r")) {
			raw = true;
		} else {
			Usage();
			return -1;
		}
	}
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
	ParseFile(fin, fout, raw);
	fin.close();
	fout.close();


#ifdef WIN32
	system("pause");
#endif

	return 0;
}

void ParseFile(std::ifstream &fin, std::ofstream &fout, bool raw)
{
	char line[128] = "";
	std::string textout = "";
	std::string dataout = "";
	int invalidCode = 0;
	int lineNumber = 0;
	std::string tmpTextOut = "";
	std::vector<Instruction*> Instructions;
	Instruction* tmp = 0;
	std::map<std::string, unsigned int> labels;
	std::map<std::string, bool> datalabel;
	int textPos = 0;


	while(!fin.eof()) {
		tmpTextOut = "";
		tmp = 0;
		fin.getline(line,sizeof(line) - 1);
		lineNumber++;
		tmp = Instruction::CreateInstruction(line);
		if(tmp) {
			tmp->ParseArguments();
			if(!tmp->IsValid()) {
				std::cerr << "Error on line " << lineNumber  << ": Invalid Argument" << std::endl;
				invalidCode++;
			}
			if(!tmp->IsDataDef() && !tmp->IsLabelDef())
				textPos += tmp->GetBinaryLen();
			Instructions.push_back(tmp);
			if(tmp->IsLabelDef() || tmp->IsDataDef()) {
				if(labels.find(tmp->GetLabelDefName()) == labels.end()) {
					labels[tmp->GetLabelDefName()] = tmp->IsLabelDef() ? textPos : dataout.size();
					datalabel[tmp->GetLabelDefName()] = tmp->IsDataDef();
					if(tmp->IsDataDef())
						dataout.append(tmp->ToBinary());
				} else {
					invalidCode++;
					std::cerr << "Error on line " << lineNumber << ": Label " << tmp->GetLabelDefName() << " already defined." << std::endl;
					continue;
				}
			}
		} else {
			continue;
		}

	}

	for(std::map<std::string, unsigned int>::iterator it = labels.begin(); it != labels.end(); it++) {
		if(datalabel[it->first]) {
			it->second = textPos + it->second;
		}
	}

	bool valid = true;
	for(unsigned int i = 0; i < Instructions.size(); i++) {
		Instructions[i]->SubstituteLabels(labels);
		valid = valid && Instructions[i]->IsValid();
		if(!Instructions[i]->IsValid()) {
			std::cerr << "Error on line: " << Instructions[i]->GetLine() << std::endl;
			invalidCode++;
		}
		if(valid && Instructions[i]->IsText())
			textout.append(Instructions[i]->ToBinary());
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
