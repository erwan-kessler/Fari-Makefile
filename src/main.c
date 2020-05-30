#include "Parser.h"
#include "Set.h"
#include "launcher.h"
#include "Commands.h"
#include "json_logs.h"

int main(int argc, char **argv) {
	char *command;
	char *filename;
	uint64_t *iterator;
	int continuationOnError = 0; // Will be set to 1 if we have -k flag

	int *errorCode = malloc(sizeof(int)); // Used to catch error code during continuationOnError
	*errorCode=0;

	int flagError=0; // Will be set to 1 if an error was spotted during the compilation


    //This part is very messy there is prolly smthg cleaner but whatever
    //We do not handle the fact that the user might enter smthg stupid either

	if (argc == 1 || (argc==2 && strcmp(argv[1],"-k")==0)) {
		filename = "farifile";
		if((argc==2 && strcmp(argv[1],"-k")==0)){
            //printf("Continuation on error enabled\n");
            continuationOnError = 1;
        }
	} else {
	    if( argc == 3 || (argc == 2 && strcmp(argv[1],"-k")==0) ){

            //printf("Continuation on error enabled\n");
	        continuationOnError = 1;

	        filename = argv[2];
	    }
	    else{
            filename = argv[1];
	    }
	}

	// parse the filename according to the rules given
	data_t *data = parser(filename);
	uint64_t *testIterator = get_iterator_set(data->source_files);
	char * temp;
	if ((temp=next_set(data->source_files, testIterator))==NULL){
		free(testIterator);
		goto linked;
	}
	char *dot = strrchr(temp, '.');
	free(testIterator);

	if (dot && !strcmp(dot, ".c")) { //If we are trying to compile a C project
		// create the commands to compile the .c to .o
		// Only commands that are needed will be built
		linked:;
        set_t *commands = createCompileCommand(data,continuationOnError,errorCode);
		// launch the compile command for the .c to .o
		iterator = get_iterator_set(commands);
		while ((command = next_set(commands, iterator)) != NULL) {
		    *errorCode = launch("gcc", command,continuationOnError);
            if(*errorCode){
                flagError = *errorCode; //If an error happend we store the errorCode to spit it ou at the end
            }
		}
		free(iterator);
		unsigned long maxTimeO = checkOFiles(data->source_files,continuationOnError,errorCode);

		//checking if we have to rebuild the executable
		char *executableName = data->executable_name;
		struct stat *buffer = malloc(sizeof(struct stat));
		int flag = 0;
		if (stat(executableName, buffer) == 0) {// If stat call went well
			if (maxTimeO > buffer->st_mtim.tv_sec) {
				//If any .o is newer than the previous executable
				flag = 1;
			}
		} else { // stat call didnt go well, the executable probably doesnt exist, we recompile
			flag = 1;
		}
		if (flag) { //if executable do need to be rebuilt
			command = createPackCommand(data);
			*errorCode = launch("gcc", command,continuationOnError);
			if(*errorCode){
                flagError = *errorCode; //If an error happend we store the errorCode to spit it ou at the end
			}
		}

		free(command);
		delete_set(commands);
		free(buffer);

        char *logs = create_log(data,continuationOnError,errorCode);
        FILE *log = fopen("logs.json", "wb");
        if (log != NULL) {
            fputs(logs, log);
            fclose(log);
        }
        free(logs);

	} else {// We are trying to compile a Java project
		if (checkCompileJava(data->source_files,continuationOnError,errorCode) == 0) {
			//We do not have to recompile since nothing changed
			free(data);
		} else { // We have to recompile
			command = createJavaCommand(data);
			*errorCode = launch("javac", command,continuationOnError);
            if(*errorCode){
                flagError = *errorCode; //If an error happends we store the errcrCode to spit it ou at the end
            }
			free(command);
		}
	}

	free(errorCode);
	destroy_data(data);
	if(continuationOnError && flagError){
	    printf("Something went wrong during the compilation.\n");
	    exit(flagError);
	}

	printf("Compilation terminée.\n");

}