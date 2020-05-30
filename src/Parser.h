#ifndef FARI_PARSER_H
#define FARI_PARSER_H
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "Set.h"

typedef struct data{
	char * executable_name;
	set_t* source_files;
	set_t* header_files;
	set_t* flags;
	set_t* libraries;
	set_t* linked_headers;
	
} data_t;

data_t *parser(char * filename);

void destroy_data(data_t *data);
int checkCorJ(char * filename);

#endif //FARI_PARSER_H
