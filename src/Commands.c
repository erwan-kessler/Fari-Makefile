#include "Commands.h"

char *createSimpleCompileCommand(char *filename, char *flags) {

	char *newName = transformCinO(filename);
	size_t lenFlags;
	if (flags != NULL) {
		lenFlags = strlen(flags);
	} else {
		lenFlags = 0;
	}
	int k = 0;
	size_t lenFilename = strlen(filename);
	size_t lenNewFilename = strlen(newName);
	char *command = malloc(sizeof(char) *
						   (3 + 1 + (flags != NULL ? lenFlags + 1 : 0) + 2 + 1 + lenFilename + 1 + 2 + 1 +
							lenNewFilename + 1));
	command[k++] = 'g';
	command[k++] = 'c';
	command[k++] = 'c';
	command[k++] = ' ';
	if (flags != NULL) {
		for (size_t i = 0; i < lenFlags; ++i) {
			command[k++] = flags[i];
		}
		command[k++] = ' ';
	}
	command[k++] = '-';
	command[k++] = 'c';
	command[k++] = ' ';
	for (size_t i = 0; i < lenFilename; ++i) {
		command[k++] = filename[i];
	}
	command[k++] = ' ';
	command[k++] = '-';
	command[k++] = 'o';
	command[k++] = ' ';
	for (size_t i = 0; i < lenNewFilename; ++i) {
		command[k++] = newName[i];
	}
	free(newName);
	command[k] = '\0'; //unnecessary but better
	return command;
}


char *createSimplePackCommand(char *filename, char *libraries, char *executable_name, char *flags) {
	size_t lenLibs;
	if (libraries != NULL) {
		lenLibs = strlen(libraries);
	} else {
		lenLibs = 0;
	}
	size_t lenFlags;
	if (flags != NULL) {
		lenFlags = strlen(flags);
	} else {
		lenFlags = 0;
	}
	int k = 0;
	size_t lenFilename = filename!=NULL?strlen(filename):0;
	size_t lenExecName = strlen(executable_name);
	char *command = malloc(sizeof(char) *
						   (3 + 1 + (flags != NULL ? lenFlags + 1 : 0) + lenFilename + 1 + 2 + 1 + lenExecName + 1 +
							(libraries != NULL ? lenLibs + 1 : 0)));
	command[k++] = 'g';
	command[k++] = 'c';
	command[k++] = 'c';
	command[k++] = ' ';
	if (flags != NULL) {
		for (size_t i = 0; i < lenFlags; ++i) {
			command[k++] = flags[i];
		}
		command[k++] = ' ';
	}
	command[k++] = '-';
	command[k++] = 'o';
	command[k++] = ' ';
	for (size_t i = 0; i < lenExecName; ++i) {
		command[k++] = executable_name[i];
	}
	command[k++] = ' ';
	for (size_t i = 0; i < lenFilename; ++i) {
		command[k++] = filename[i];
	}
	if (libraries != NULL) {
		command[k++] = ' ';

		for (size_t i = 0; i < lenLibs; ++i) {
			command[k++] = libraries[i];
		}
	}
	command[k] = '\0'; //unnecessary but better
	return command;
}

char *generateStringFromSet(set_t *set, char *(*functor)(char *)) {
	//this could be in utils or in the set but for now its only needed here so it should not be factorized yet
	uint64_t *iterator = get_iterator_set(set);
	char *value;
	char *final = NULL;
	size_t temp = 0;
	while ((value = next_set(set, iterator)) != NULL) {
		value = functor != NULL ? functor(value) : value;
		if (final == NULL) {
			final = (char *) calloc(strlen(value) + 1, sizeof(char));
			strcpy(final, value);
		} else {
			final = (char *) realloc(final, sizeof(char) * (temp + 2 + strlen(value)));
			final[temp] = ' '; //remove the \0 at the end and replace it with a space
			strcpy(final + temp + 1, value); //copy at the index 0 + last known index + 1 for space
		}
		temp = strlen(final);
		if (functor != NULL) {
			free(value); //because the functor will always give a copy
		}
	}
	free(iterator);
	return final;
}

set_t *createCompileCommand(data_t *data, int continuationOnError, int *errorCode) {

	unsigned long maxTH = checkHeaders(data->header_files, continuationOnError, errorCode);
	char *flags = generateStringFromSet(data->flags, NULL);
	set_t *toRebuild = checkRebuild(data->source_files, maxTH, continuationOnError, errorCode);
	unsigned long maxTHLinked = checkLinkedHeaders(data->linked_headers, continuationOnError, errorCode);
	set_t *toRebuildLinked = checkRebuildLinked(data->linked_headers, maxTHLinked, continuationOnError, errorCode);
	uint64_t *iteratorToRebuild = get_iterator_set(toRebuild);
	uint64_t *iteratorToRebuildLinked = get_iterator_set(toRebuildLinked);
	set_t *set = create_set();
	char *value;
	while ((value = next_set(toRebuild, iteratorToRebuild)) != NULL) {
		char *command = createSimpleCompileCommand(value, flags);
		insert_set(set, command);
		free(command);
	}
	while ((value = next_set(toRebuildLinked, iteratorToRebuildLinked)) != NULL) {
		char *command = createSimpleCompileCommand(value, flags);
		insert_set(set, command);
		free(command);
	}
	delete_set(toRebuild);
	delete_set(toRebuildLinked);
	free(iteratorToRebuild);
	free(iteratorToRebuildLinked);
	free(flags);
	return set;
}

int checkIfC(char* string){
	if (string==NULL){
		return 0;
	}
	char *dot = strrchr(string, '.');
	return dot && !strcmp(dot, ".c");
}
char *createPackCommand(data_t *data) {
	set_t* globalCFiles=union_set_on_condition(data->source_files,data->linked_headers,checkIfC);
	char *objectFiles = generateStringFromSet(globalCFiles, transformCinO);
	char *libraries = generateStringFromSet(data->libraries, NULL);
	char *flags = generateStringFromSet(data->flags, NULL);
	char *res = createSimplePackCommand(objectFiles, libraries, data->executable_name, flags);
	delete_set(globalCFiles);
	free(objectFiles);
	free(libraries);
	free(flags);
	return res;
}

char *createSimpleJavaCommand(char *javaFiles, char *javaFlags, char *javaLibs) {

	size_t javaLen = 0;
	if (javaFiles != NULL) {
		javaLen = strlen(javaFiles);
	}
	size_t libsLen = 0;
	if (javaLibs != NULL) {
		libsLen = strlen(javaLibs);
	}
	size_t flagsLen = 0;
	if (javaFlags != NULL) {
		flagsLen = strlen(javaFlags);
	}

	int k = 0;
	char *command = malloc((5 + flagsLen + javaLen + libsLen + 3) * sizeof(char));
	command[k++] = 'j';
	command[k++] = 'a';
	command[k++] = 'v';
	command[k++] = 'a';
	command[k++] = 'c';
	if (flagsLen != 0) {
		command[k++] = ' ';
		for (size_t i = 0; i < flagsLen; ++i) {
			command[k++] = javaFlags[i];
		}
	}

	if (libsLen != 0) {
		command[k++] = ' ';
		for (size_t i = 0; i < libsLen; ++i) {
			command[k++] = javaLibs[i];
		}
	}

	if (javaLen != 0) {
		command[k++] = ' ';
		for (size_t i = 0; i < javaLen; ++i) {
			command[k++] = javaFiles[i];
		}
	}
	command[k] = '\0';
	return command;
}

char *createJavaCommand(data_t *data) {

	char *javaFiles = generateStringFromSet(data->source_files, NULL);
	char *javaFlags = generateStringFromSet(data->flags, NULL);
	char *javaLibs = generateStringFromSet(data->libraries, NULL);
	char *res = createSimpleJavaCommand(javaFiles, javaFlags, javaLibs);
	free(javaFiles);
	free(javaFlags);
	free(javaLibs);
	return res;

}