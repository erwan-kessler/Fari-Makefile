#ifndef FARI_CHECKER_H
#define FARI_CHECKER_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include "Set.h"
#include "../highwayhash/highway.h"


unsigned long checkHeaders(set_t * set, int continuationOnError, int * errorCode);
unsigned long checkOFiles(set_t * set, int continuationOnError, int * errorCode);
set_t * checkRebuild(set_t * setC, unsigned long maxTimeH, int continuationOnError,int *errorCode);
char *  transformCinO(char * filename);
char* transformJavaInClass(char*filename);
int checkCompileJava(set_t *setJava, int continuationOnError, int * errorCode);
unsigned long checkLinkedHeaders(set_t *set, int continuationOnError, int *errorCode);
set_t * checkRebuildLinked(set_t * setC, unsigned long maxTimeH, int continuationOnError,int *errorCode);

#endif //FARI_CHECKER_H
