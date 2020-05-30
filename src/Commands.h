#ifndef FARI_COMMANDS_H
#define FARI_COMMANDS_H
#include "Parser.h"
#include "Checker.h"

set_t *createCompileCommand(data_t *data, int continuationOnError, int * errorCode);
char *createPackCommand(data_t *data);
char * createJavaCommand(data_t * data);

#endif //FARI_COMMANDS_H
