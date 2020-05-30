#include "Checker.h"
#include <errno.h>

unsigned long checkHeaders(set_t *set, int continuationOnError, int *errorCode) {
    //returns max of time since last change in a header file
    //errorCode will hold the stat errorValue if we are using continuationOnError


    int nbHeaders = set->number_of_elements;
    uint64_t *iterator = get_iterator_set(set);

    time_t maxTime = 0;
    int error = 0;

    struct stat *buffer = malloc(sizeof(struct stat));

    for (int i = 0; i < nbHeaders - 1; ++i) {

        char *headerName = next_set(set, iterator);
        if ((error = stat(headerName, buffer)) == 0) { // If stat call went well

            if (buffer->st_mtim.tv_sec > maxTime) {
                maxTime = buffer->st_mtim.tv_sec;
            }
        } else { // Stat didnt go well
            if (continuationOnError) {
                *errorCode = error;
            } else {
                fprintf(stderr, "Error during stat call, %s %s\n", strerror(errno), headerName);
                free(iterator);
                free(buffer);
                exit(error);
            }
        }
    }
    free(iterator);
    free(buffer);
    return maxTime;
}


unsigned long checkLinkedHeaders(set_t *set, int continuationOnError, int *errorCode) {
    //returns max of time since last change in a header file
    //errorCode will hold the stat errorValue if we are using continuationOnError

    uint64_t *iterator = get_iterator_set(set);

    time_t maxTime = 0;
    int error = 0;
    char *value;
    struct stat *buffer = malloc(sizeof(struct stat));

    while ((value = next_set(set, iterator)) != NULL) {
        if (value[strlen(value) - 1] == 'h') {
            if ((error = stat(value, buffer)) == 0) { // If stat call went well

                if (buffer->st_mtim.tv_sec > maxTime) {
                    maxTime = buffer->st_mtim.tv_sec;
                }
            } else { // Stat didnt go well
                if (continuationOnError) {
                    *errorCode = error;
                } else {
                    fprintf(stderr, "Error during stat call, %s %s\n", strerror(errno), value);
                    free(iterator);
                    free(buffer);
                    exit(error);
                }
            }
        }
    }

    free(iterator);
    free(buffer);
    return maxTime;
}

unsigned long checkOFiles(set_t *set, int continuationOnError, int *errorCode) {
    //returns the max of st_mtime of .o files linked to a set of .c files
    //errorCode will hold the stat errorValue if we are using continuationOnError

    int nbFiles = set->number_of_elements;
    uint64_t *iterator = get_iterator_set(set);

    time_t maxTime = 0;
    int error = -9;
    struct stat *buffer = malloc(sizeof(struct stat));

    for (int i = 0; i < nbFiles - 1; ++i) {

        char *fileName = next_set(set, iterator);
        char *fileNameO = transformCinO(fileName);

        if ((error = stat(fileNameO, buffer)) == 0) { // If stat call went well
            if (buffer->st_mtim.tv_sec > maxTime) {
                maxTime = buffer->st_mtim.tv_sec;
            }
        } else { // Stat didnt go well
            if (continuationOnError) {
                *errorCode = error;
            } else {
                fprintf(stderr, "Error during stat call, %s %s\n", strerror(errno), fileNameO);
                free(iterator);
                free(buffer);
                free(fileNameO);
                exit(error);
            }
        }
        free(fileNameO);

    }
    free(iterator);
    free(buffer);
    return maxTime;

}

set_t *checkRebuildLinked(set_t *setC, unsigned long maxTimeH, int continuationOnError, int *errorCode) {
    //checks whether we have to recompile a .c file or not
    //errorCode will hold the stat errorValue if we are using continuationOnError

    set_t *set = create_set();

    int nbC = setC->number_of_elements;
    int error = 0;

    uint64_t *iteratorC = get_iterator_set(setC);

    struct stat *bufferC = malloc(sizeof(struct stat));
    struct stat *bufferO = malloc(sizeof(struct stat));

    unsigned long timeC = 0;

    for (int i = 0; i < nbC - 1; ++i) {
        char *cName = next_set(setC, iteratorC);
        if (cName[strlen(cName) - 1] == 'c') {
            if ((error = stat(cName, bufferC)) == 0) { // If stat call went well
                timeC = bufferC->st_mtim.tv_sec;
                if (timeC <= maxTimeH) {
                    //If any header file was modified after the .c file we need to recompile
                    int flag = insert_set(set, cName);
                    if (flag != 0) { //this should never ever happen since that would mean the set doesnt work
                        free(iteratorC);
                        free(bufferC);
                        free(bufferO);
                        exit(flag);
                    }
                } else {
                    char *cNameOFile = transformCinO(cName);
                    if ((error = stat(cNameOFile, bufferO)) < 0 || bufferO->st_mtim.tv_sec < timeC) {
                        //If the .o file is older than the .c file, we need to recompile
                        int flag = insert_set(set, cName);
                        if (flag != 0) { //this should never ever happen since that would mean the set doesnt work
                            free(cNameOFile);
                            free(iteratorC);
                            free(bufferC);
                            free(bufferO);
                            exit(flag);
                        }
                    }
                    free(cNameOFile);
                }
            } else { // stat call didnt go well
                if (continuationOnError) {
                    *errorCode = error;

                } else {

                    fprintf(stderr, "Error during stat call, %s %s\n", strerror(errno), cName);
                    free(iteratorC);
                    free(bufferC);
                    free(bufferO);
                    exit(-9);
                }
            }
        }

    }
    free(iteratorC);
    free(bufferC);
    free(bufferO);
    return set;
}

set_t *checkRebuild(set_t *setC, unsigned long maxTimeH, int continuationOnError, int *errorCode) {
    //checks whether we have to recompile a .c file or not
    //errorCode will hold the stat errorValue if we are using continuationOnError

    set_t *set = create_set();

    int nbC = setC->number_of_elements;
    int error = 0;

    uint64_t *iteratorC = get_iterator_set(setC);

    struct stat *bufferC = malloc(sizeof(struct stat));
    struct stat *bufferO = malloc(sizeof(struct stat));

    unsigned long timeC = 0;

    for (int i = 0; i < nbC - 1; ++i) {
        char *cName = next_set(setC, iteratorC);

        if ((error = stat(cName, bufferC)) == 0) { // If stat call went well
            timeC = bufferC->st_mtim.tv_sec;
            if (timeC <= maxTimeH) {
                //If any header file was modified after the .c file we need to recompile
                int flag = insert_set(set, cName);
                if (flag != 0) { //this should never ever happen since that would mean the set doesnt work
                    free(iteratorC);
                    free(bufferC);
                    free(bufferO);
                    exit(flag);
                }
            } else {
                char *cNameOFile = transformCinO(cName);
                if ((error = stat(cNameOFile, bufferO)) < 0 || bufferO->st_mtim.tv_sec < timeC) {
                    //If the .o file is older than the .c file, we need to recompile
                    int flag = insert_set(set, cName);
                    if (flag != 0) { //this should never ever happen since that would mean the set doesnt work
                        free(cNameOFile);
                        free(iteratorC);
                        free(bufferC);
                        free(bufferO);
                        exit(flag);
                    }
                }
                free(cNameOFile);
            }
        } else { // stat call didnt go well
            if (continuationOnError) {
                *errorCode = error;

            } else {

                fprintf(stderr, "Error during stat call, %s %s\n", strerror(errno), cName);
                free(iteratorC);
                free(bufferC);
                free(bufferO);
                exit(-9);
            }
        }
    }
    free(iteratorC);
    free(bufferC);
    free(bufferO);
    return set;
}

char *transformCinO(char *filename) { //changes the extension of a given filename to a .o file
    char *newStr = malloc(strlen(filename) + 1);
    strcpy(newStr, filename);
    char *lastDot = strrchr(newStr, '.');
    *(lastDot + 1) = 'o';
    return newStr;

}

char *transformJavaInClass(char *filename) {//changes the extension of a given file to a .class file

    char *newStr = malloc(strlen(filename) + 3);
    strcpy(newStr, filename);
    char *lastdot = strrchr(newStr, '.');
    strcpy(lastdot, ".class");
    newStr[strlen(newStr) + 1] = '\0';
    return newStr;
}

int checkCompileJava(set_t *setJava, int continuationOnError, int *errorCode) {
    // Checks if we have to recompile a project given a list of java files
    // errorCode will hold the stat errorValue if we are using continuationOnErro

    int flag = 0;
    int error;
    uint64_t *iterator = get_iterator_set(setJava);
    char *javaName;

    while ((javaName = next_set(setJava, iterator)) != NULL) {
        struct stat *bufferJava = malloc(sizeof(struct stat));
        struct stat *bufferClass = malloc(sizeof(struct stat));
        printf("%s\n", javaName);

        if ((error = stat(javaName, bufferJava)) == 0) {//If stat call went well

            char *className = transformJavaInClass(javaName);
            if ((error = stat(className, bufferClass) != 0)) {// If stat call wen well
                if (bufferJava->st_mtim.tv_sec > bufferClass->st_mtim.tv_sec) {
                    //If any .class is older than a .java, we have to recompile
                    flag = 1;
                }
            } else {// Stat call on a class file didnt go well, the file probably doesnt exist, we have to recompile
                flag = 1;
            }
            free(className);
        } else { // Stat didnt go well
            if (continuationOnError) {
                *errorCode = error;

            } else {
                fprintf(stderr, "Error during stat call, %s\n", strerror(errno));
                free(iterator);
                free(bufferJava);
                free(bufferClass);
                free(javaName);
                exit(error);
            }
        }
        free(bufferJava);
        free(bufferClass);
    }
    free(iterator);
    free(javaName);
    return flag;
}
