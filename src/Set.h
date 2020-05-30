

#ifndef FARI_SET_H
#define FARI_SET_H
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../highwayhash/highway.h"

#define HEAP_SIZE UINT16_MAX
enum error_msg {
	NOT_IN_SET = -1,
	EMPTY_SET = -2,
	END_OF_SET = -3,
	OUT_OF_BOUNDS = -4,
	TOO_MANY_ELEMENTS = -5,
	INSERT_FAILED = -6,
	NOT_IN_KEYS=-7
};

typedef struct Element {
	char *value;
	uint64_t key; //the full 64bits key is stored here
} element_t;

typedef struct List list_t;
struct List {
	element_t *current;
	list_t *next;
};

typedef struct HashMap {
	list_t **tab; // each entry is a list with elements whose hash%size is the index
	uint64_t size; //this is the size of the array and also how are trimmed the keys
} hashmap_t;

typedef struct Set {
	hashmap_t *map;
	uint64_t *keys; //the full 64bits keys are stored here
	uint64_t number_of_elements;
} set_t;

uint64_t hash(char *value);

set_t *create_set();

void delete_set(set_t *set);

int insert_set(set_t *set, char *value);

int remove_set(set_t *set, char *value);

int contains_set(set_t *set, char *value);

char * get_value_set(set_t* set, uint64_t key);

set_t * union_set(set_t *set1, set_t *set2);

set_t *union_set_on_condition(set_t *set1, set_t *set2,  int(*condition)(char *));

set_t *substract_set(set_t *set1, set_t *set2);

set_t *intersect_set(set_t *set1, set_t *set2);

uint64_t get_size_set(set_t *set);

int is_empty_set(set_t *set);

uint64_t* get_iterator_set(set_t *set);

char *next_set(set_t *set, uint64_t *pointer);


#endif //FARI_SET_H




