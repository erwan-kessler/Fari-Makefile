#ifndef FARI_JSON_LOGS_H
#define FARI_JSON_LOGS_H


#include "Parser.h"
#include "../cJson/cJSON.h"
#include "Commands.h"

char *create_log(data_t *data, int continuationOnError, int * errorCode);
#endif //FARI_JSON_LOGS_H
