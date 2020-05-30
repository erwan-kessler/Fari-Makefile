#include "launcher.h"

int launch(char *name, char*command, int continueOnError){
    //continueOnError equals 1 option is enable, then luanch returns the exitcode if shit hit the fan while compiling

    pid_t pid = fork();
    int exitcode = 0;

    if(pid<0){
        fprintf(stderr,"Error creating the process\n");
        exit(-1);
    } else{
        int status;
        if (pid==0){ //Dans le fils
        	fprintf(stderr,"%s\n",command);
        	char* tokens[256];
			tokenizeCommand(command,tokens," ");
            execvp(name,tokens);
            exit(-33);
        }
        else{ // Dans le père
            while(waitpid(pid, &status, WCONTINUED) > 0){
                if(WIFEXITED(status)){
                	exitcode=WEXITSTATUS(status);
					if (exitcode != 0 ) {
						fprintf(stderr, "gcc error\n");
						if(continueOnError==0){
                            exit(exitcode);
						} else{
                            return(exitcode);
						}
					}
					return 0;
                }
             // You should have a timeout here.. so making a counter of seconds and lets say let only 60s for the child
            }
        }
    }

    exit(-62);
}

void tokenizeCommand(char * command, char ** tokens, char * delim){
    //this function tokenizes a string according to the delimiter passed in argument
    int i = 0;
    tokens[0]= strtok(command,delim);

    while(tokens[i]!=NULL){
        ++i;
        tokens[i]=strtok(NULL,delim);
    }
}