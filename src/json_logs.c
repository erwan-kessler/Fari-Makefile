#include "json_logs.h"

void addToJsonObject(set_t *set, char*key, cJSON *log){
    char *value;
    cJSON *sources = cJSON_CreateArray();
    if (sources == NULL) {
        fprintf(stderr,"There was an error while doing the log, please try again\n");
        cJSON_Delete(log);
        exit(-106);
    }
    cJSON_AddItemToObject(log, key, sources);
    uint64_t * iteratorSource=get_iterator_set(set);
    while ((value = next_set(set, iteratorSource)) != NULL) {
        cJSON* source = cJSON_CreateString(value);
        if (source==NULL){
            fprintf(stderr,"There was an error while doing the log, please try again\n");
            cJSON_Delete(log);
            exit(-106);
        }
        cJSON_AddItemToArray(sources, source);
    }
    free(iteratorSource);
}

char *create_log(data_t *data, int continuationOnError, int * errorCode) {
    cJSON *log = cJSON_CreateObject();
    if (log == NULL) {
        goto end;
    }
    //log parts
    addToJsonObject(data->source_files,"sources",log);
    addToJsonObject(data->header_files,"headers",log);
    addToJsonObject(data->libraries,"libraries",log);
    addToJsonObject(data->flags,"flags",log);
    cJSON* executable_name = cJSON_CreateString(data->executable_name);
    if (executable_name==NULL){
        goto end;
    }
    cJSON_AddItemToObject(log, "executable_name", executable_name);

    //error msg
    cJSON* fari_error_msg = cJSON_CreateString("");
    if (fari_error_msg == NULL){
        goto end;
    }
    cJSON_AddItemToObject(log, "fari_error_msg", fari_error_msg);

    //compilation
    cJSON *compilation = cJSON_CreateObject();
    if (compilation == NULL) {
        goto end;
    }
    cJSON_AddItemToObject(log,"compilation",compilation);
    cJSON* error_msg_comp = cJSON_CreateString("");
    if (error_msg_comp == NULL){
        goto end;
    }
	set_t *temp_data=createCompileCommand(data,continuationOnError,errorCode);
    addToJsonObject(temp_data,"commands",compilation);
    delete_set(temp_data);
    cJSON_AddItemToObject(compilation, "error_msg", error_msg_comp);

	//Linking
	cJSON *linking = cJSON_CreateObject();
	if (linking == NULL) {
		goto end;
	}
	cJSON_AddItemToObject(log,"linking",linking);
	cJSON* error_msg_link = cJSON_CreateString("");
	if (error_msg_link == NULL){
		goto end;
	}
	char * temp_str=createPackCommand(data);
	cJSON* command_link = cJSON_CreateString(temp_str);
	free(temp_str);
	if (command_link == NULL){
		goto end;
	}

	cJSON_AddItemToObject(linking,"command",command_link);
	cJSON_AddItemToObject(linking, "error_msg", error_msg_link);

	//ending msg
	cJSON* fari_msg = cJSON_CreateString("Compilation terminée.");
	if (fari_msg == NULL){
		goto end;
	}
	cJSON_AddItemToObject(log, "fari_msg", fari_msg);

	//printing
    char *string = cJSON_Print(log);
    if (string == NULL) {
        fprintf(stderr, "Failed to print monitor.\n");
    }
    goto correct;
    end:
    fprintf(stderr,"There was an error while doing the log, please try again\n");
    cJSON_Delete(log);
    exit(-106);
    correct:
    cJSON_Delete(log);
    return string;
}