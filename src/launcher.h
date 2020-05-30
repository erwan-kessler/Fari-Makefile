//
// Created by yann on 28/10/2019.
//

#ifndef RS1920_KESSLER_MEYER_LAUNCHER_H
#define RS1920_KESSLER_MEYER_LAUNCHER_H


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

int launch(char *name, char*command, int continueOnError);
void tokenizeCommand(char * command, char ** tokens, char * delim);

#endif //RS1920_KESSLER_MEYER_LAUNCHER_H
