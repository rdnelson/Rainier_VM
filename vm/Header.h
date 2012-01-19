#ifndef __HEADER_H__
#define __HEADER_H__

struct RNPE_Header{
	char magic[4]; //RNpe
	unsigned int size;
	unsigned int text_pos; //ref from begin of file
	unsigned int text_size;
	unsigned int data_pos; //ref from begin of file
	unsigned int data_size;
	unsigned int entry_pos; //ref from begin of text
};

#endif
