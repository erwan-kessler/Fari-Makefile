#include "Parser.h"
#include "glob.h"


#define test 0
#define NUMBER_OF_STARTS 11
#define C 1
#define NONE 0
#define JAVA 2
char possible_starts[NUMBER_OF_STARTS] = {'B', 'C', 'H', 'E', 'F', 'J', '#', ' ', '\t', '\r', '\n'};

data_t *init_data() {
	data_t *data = (data_t *) malloc(sizeof(data_t));
	data->flags = create_set();
	data->header_files = create_set();
	data->libraries = create_set();
	data->source_files = create_set();
	data->executable_name = NULL;
	data->linked_headers=create_set();
	return data;
}


void destroy_data(data_t *data) {
	delete_set(data->source_files);
	delete_set(data->libraries);
	delete_set(data->header_files);
	delete_set(data->flags);
	delete_set(data->linked_headers);
	free(data->executable_name);
	free(data);
}

FILE *fileOpener(char *name) {
	FILE *fptr;
	fptr = fopen(name, "r");
	if (fptr == NULL) {
		fprintf(stderr, "File %s could not be opened, please check if the file do exist. \n Error: %s\n", name, strerror(errno));
		exit(1);
	}
	return fptr;
}


int is_correct_start(char start) {
	for (int i = 0; i < NUMBER_OF_STARTS; ++i) {
		if (start == possible_starts[i]) {
			return 1;
		}
	}
	return 0;
}

int parseLine(set_t *set, char *line, char *sanitizer, int firstChar, char *path_sub) {
	char *pch;
	strtok(line, " \r\n"); //remove the identifier
	pch = strtok(NULL, " \r\n");
	while (pch != NULL) {
		if (firstChar) {
			for (size_t i = 0; i < strlen(sanitizer); ++i) {
				if (pch[i] != sanitizer[i]) {
					return 1;
				}
			}
		} else {
			for (size_t i = strlen(sanitizer); i > 0; --i) {
				if (pch[strlen(pch) - i] != sanitizer[strlen(sanitizer) - i]) {
					return 1;
				}
			}
		}
		char *path_sub_cpy = calloc(strlen(path_sub) + 1 + strlen(pch) + 1, sizeof(char));
		path_sub_cpy = strcpy(path_sub_cpy, path_sub);
		path_sub_cpy = strcat(path_sub_cpy, pch);
		
		if(strchr(path_sub_cpy,'*')!=NULL){
		    glob_t * globList = malloc(sizeof(glob_t));
            glob(path_sub_cpy,0,NULL, globList);
            for (size_t i = 0; i < globList->gl_pathc; ++i) {
                insert_set(set,globList->gl_pathv[i]);
            }
		    globfree(globList);
		    free(globList);
		}
		else{
            insert_set(set, path_sub_cpy);
		}

		free(path_sub_cpy);
		pch = strtok(NULL, " \r\n");

	}
	return 0;
}

void free_parser_internal(FILE *file, data_t *data, char *line, char *path_sub) {
	fclose(file);
	destroy_data(data);
	free(line);
    if (strcmp(path_sub,"")!=0){
        free(path_sub);
    }
}

int checkCorJ(char *filename) {
	FILE *file2 = fileOpener(filename);
	char *line = NULL;
	size_t longeur = 0;
	int flag = NONE;

	while (getline(&line, &longeur, file2) != -1) {
		if (!is_correct_start(line[0])) {
			fprintf(stderr, "The start of the line is incorrect, got %c ,ascii code: %d \n", line[0], (int) line[0]);
			exit(-1);
		}
		if (line[0] == 'J') {
			if (flag == 1) {
				fprintf(stderr, "Cannot compile both C and Java files in the same command");
				exit(-43);
			}
			flag = JAVA;
		} else {
			if (line[0] == 'C') {
				if (flag == JAVA) {
					fprintf(stderr, "Cannot compile both C and Java files in the same command");
					exit(-43);
				}
				flag = C;
			}
		}
	}
	fclose(file2);
	free(line);
	return flag;

}

char *return_path_sub(char *filename) {
    char* path_sub;
	if (filename[0]!='/'){
        path_sub = calloc(2, sizeof(char));
        path_sub[0] = '.';
        path_sub[1] = '\0';
	}
	else{
        path_sub = calloc(1, sizeof(char));
        path_sub[0] = '\0';
	}
	char *pch = NULL;
	char *filename_copy = calloc(strlen(filename) + 1, sizeof(char));
	filename_copy = strcpy(filename_copy, filename);
	pch = strtok(filename_copy, "/");
	int cmp=0;
	while (pch != NULL) {
		path_sub = realloc(path_sub, (strlen(path_sub) + 1 + strlen(pch) + 1 + 1) * sizeof(char));
		strcat(path_sub, "/");
		char *copy = pch;
		if ((pch = strtok(NULL, "/")) != NULL) {
			strcat(path_sub, copy);
		}
		cmp++;
	}
	free(filename_copy);
	if (cmp<2){
	    free(path_sub);
		return "";
	}
	return path_sub;
}

data_t *parser(char *filename) {

	FILE *file = fileOpener(filename);

	char *line = NULL;
	char *pch = NULL;
	size_t longeur = 0;

	data_t *data = init_data();
	char *path_sub = "";//return_path_sub(filename); //precision dans le sujet
	int flag = checkCorJ(filename); // Sets to 1 if we find a C line and 2 if we find a J line
	while (getline(&line, &longeur, file) != -1) {
		if (!is_correct_start(line[0])) {
			fprintf(stderr, "The start of the line is incorrect, got %c ,ascii code: %d \n", line[0], (int) line[0]);
			free_parser_internal(file, data, line, path_sub);
			exit(-1);
		}

		switch (line[0]) {
			case 'B':
				if (flag == C) { //If we are in a C type farifile
					if (parseLine(data->libraries, line, "-", 1, "")) {
						free_parser_internal(file, data, line, path_sub);
						fprintf(stderr, "The line contains one or more incorrect type of libraries\n");
						exit(-10);
					}
				} else { //If we are in a Java type farifile
					if (parseLine(data->libraries, line, "", 1, "")) {
						free_parser_internal(file, data, line, path_sub);

						fprintf(stderr, "The line contains one or more incorrect type of libraries\n");
						exit(-10);
					}
				}
				break;

			case 'C':
				if (flag == JAVA) {
					fprintf(stderr, "Cannot compile both C and Java files in the same command");
					exit(-132);
				} else {
					if (line[1] == 'H') {
						if (parseLine(data->linked_headers, line+1, "", 0, path_sub)) {
							free_parser_internal(file, data, line, path_sub);
							fprintf(stderr, "The line contains one or more incorrect type of linked source file\n");
							exit(-11);
						}
					}
					if (parseLine(data->source_files, line, ".c", 0, path_sub)) {
						free_parser_internal(file, data, line, path_sub);
						fprintf(stderr, "The line contains one or more incorrect type of source file\n");
						exit(-11);
					}

				}
				break;

			case 'J':

				if (flag == C) {
					fprintf(stderr, "Cannot compile both C and Java files in the same command");
					exit(-132);
				} else {
					if (parseLine(data->source_files, line, ".java", 0, path_sub)) {
						free_parser_internal(file, data, line, path_sub);
						fprintf(stderr, "The line contains one or more incorrect type of source file\n");
						exit(-11);
					}
				}

				break;

			case 'F':

				if (flag == C) {
					if (parseLine(data->flags, line, "", 1, "")) {
						free_parser_internal(file, data, line, path_sub);
						fprintf(stderr, "The line contains one or more incorrect type of flags\n");
						exit(-12);
					}
				} else {
					if (parseLine(data->flags, line, "", 1, "")) {
						free_parser_internal(file, data, line, path_sub);
						fprintf(stderr, "The line contains one or more incorrect type of libraries\n");
						exit(-10);
					}
				}

				break;
			case 'H':
				if (flag == JAVA) {
					fprintf(stderr, "A java farifile shouldnt contain a header line");
					exit(-53);

				} else {
					if (parseLine(data->header_files, line, ".h", 0, path_sub)) {
						free_parser_internal(file, data, line, path_sub);
						fprintf(stderr, "The line contains one or more incorrect type of header file\n");
						exit(-13);

					}
				}
				break;
			case 'E':
				if (flag == JAVA) {
					fprintf(stderr, "A java farifile shouldnt contain an executable line\n");
					exit(-52);

				} else {
					strtok(line, " "); //remove the identifier E
					if (data->executable_name == NULL) {
						pch = strtok(NULL, " \r\n");
						if (pch == NULL) {
							//case where we have only E on the line
							fprintf(stderr, "No name was provided for the executable, exiting\n");
							free_parser_internal(file, data, line, path_sub);
							exit(-3);
						}
						data->executable_name = strdup(pch);
						pch = strtok(NULL, " \r\n");
						if (pch != NULL) {
							//case where we have multiple name for the executable
							while (pch != NULL) {
								fprintf(stderr, "The line contains one or more name here the follow-up: %s\n", pch);
								pch = strtok(NULL, " \r\n");
							}
							fprintf(stderr, "Exiting...\r");
							free_parser_internal(file, data, line, path_sub);
							exit(-7);
						}
					} else {
						fprintf(stderr, "The executable name was already assigned, exiting\n");
						free_parser_internal(file, data, line, path_sub);
						exit(-2);
					}
				}
				break;
			default:
				break;
		}
	}
	if (data->executable_name == NULL && flag != JAVA) {
		fprintf(stderr, "No filename was given, exiting...\n");
		free_parser_internal(file, data, line, path_sub);
		exit(-6);
	}
	fclose(file);
	free(line);
	if (strcmp(path_sub,"")!=0){
        free(path_sub);
	}
	return data;
}
