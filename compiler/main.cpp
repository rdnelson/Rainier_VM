#include <iostream>
#include <cstring>
#include <fstream>
#include "Header.h"

const char MAGIC_CONST[] = "RNpe";

int main()
{
	std::cout << sizeof(RNPE_Header) << std::endl;
	RNPE_Header head;
	memset(&head, 0, sizeof(RNPE_Header));
	memcpy(head.magic, MAGIC_CONST, sizeof(head.magic));
	head.size = sizeof(RNPE_Header);
	head.data_pos = sizeof(RNPE_Header);
	head.text_pos = sizeof(RNPE_Header);

	std::ofstream fout("test.rnpe");
	fout.write((char*)&head, sizeof(head));
	fout.close();
	return 0;
}
