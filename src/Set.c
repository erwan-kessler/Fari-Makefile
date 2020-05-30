#include "Set.h"

/*
 * This define a hash of any string into a 64bits number, only UINT64_MAX is never reached.
 */
uint64_t hash(char *value) {
	uint64_t key[4] = {1, 2, 3, 4};
	size_t size = strlen(value);
	uint64_t hash = HighwayHash64((const uint8_t *) value, size, key);
	return (hash != UINT64_MAX ? hash : 0);
}

/*
 * This create a set with the particularity to have always one Element allocated but not
 * defined for each list of the heap, this allow to fill it directly in the next methods.
 * A set will always have the UINT64_MAX key as the provisional for the new one.
 */
set_t *create_set() {
	set_t *set = (set_t *) malloc(sizeof(set_t));
	set->keys = (uint64_t *) calloc(1, sizeof(uint64_t));
	set->keys[0] = UINT64_MAX;
	set->map = (hashmap_t *) malloc(sizeof(hashmap_t));
	set->map->size = HEAP_SIZE;
	set->map->tab = (list_t **) calloc(HEAP_SIZE, sizeof(list_t *));
	set->number_of_elements = 1;
	for (uint64_t i = 0; i < set->map->size; ++i) {
		set->map->tab[i] = (list_t *) malloc(sizeof(list_t));
		set->map->tab[i]->current = (element_t *) malloc(sizeof(element_t));
		set->map->tab[i]->next = NULL;
		set->map->tab[i]->current->key = UINT64_MAX;
		set->map->tab[i]->current->value = NULL;
	}
	return set;
}

void delete_set(set_t *set) {
	for (uint64_t i = 0; i < set->map->size; ++i) {
		list_t *head = set->map->tab[i];
		while (head->next != NULL) {
			free(head->current->value);
			free(head->current);
			list_t *temp = head->next;
			free(head);
			head=temp;
		}
        free(head->current->value);
        free(head->current);
		free(head);
	}
	free(set->map->tab);
	free(set->map);
	free(set->keys);
	free(set);
}

int __resize_hashmap(hashmap_t *hashmap) {
	if (hashmap->size > (UINT64_MAX / UINT16_MAX)) {
		//we can go higher than this threshold but we might have too many collisions already
		return TOO_MANY_ELEMENTS;
	}
	uint64_t new_size = hashmap->size + HEAP_SIZE;
	// Allocate a new array of list, we cannot do it in place else we would have issue with
	// elements going back into old lists.
	list_t **new_tab = (list_t **) calloc(new_size, sizeof(list_t *));
	for (uint64_t i = 0; i < new_size; ++i) {
		new_tab[i] = (list_t *) malloc(sizeof(list_t));
		new_tab[i]->current = (element_t *) malloc(sizeof(element_t));
		new_tab[i]->next = NULL;
		new_tab[i]->current->key = UINT64_MAX;
		new_tab[i]->current->value = NULL;
	}

	for (uint64_t i = 0; i < hashmap->size; ++i) {
		list_t *head = hashmap->tab[i];
		while (head->next != NULL) {
			// Get the correct entry in the new hashmap with the new key
			// resized to the new size
			list_t *temp_head = new_tab[head->current->key % new_size];
			// Append at the end of the list the new element
			while (temp_head->next != NULL) {
				temp_head = temp_head->next;
			}
			// Copy in the temporary cell left
			assert(temp_head->current->value == NULL);
			assert(temp_head->current->key == UINT64_MAX);
			temp_head->current->value = strdup(head->current->value);
			temp_head->current->key = head->current->key;

			// Append a new empty cell
			temp_head->next = (list_t *) malloc(sizeof(list_t));
			temp_head->next->current = (element_t *) malloc(sizeof(element_t));
			temp_head->next->current->value = NULL;
			temp_head->next->current->key = UINT64_MAX;
			temp_head->next->next = NULL;

			//free the old values
			free(head->current->value);
			free(head->current);
			list_t* temp = head->next;
			free(head);
			head=temp;
		}
        free(head->current->value);
        free(head->current);
		free(head);
	}
	free(hashmap->tab);
	hashmap->tab = new_tab;
	hashmap->size = new_size;
	return 0;
}

int insert_set(set_t *set, char *value) {
	uint64_t hashed_value = hash(value);
	// if the filling of the hashmap is more than 2 thirds then we resize it
	if (set->number_of_elements + 1 > (set->map->size * 2 / 3)) {
		int error = __resize_hashmap(set->map);
		if (error == TOO_MANY_ELEMENTS) {
			fprintf(stderr, "The HashMap could not be resized, the element was not inserted\n");
			return INSERT_FAILED;
		}
	}

	list_t *head = set->map->tab[hashed_value % set->map->size];
	while (head->next != NULL) {
		if (head->current->key == hashed_value) {
			if (strcmp(head->current->value, value) == 0) {
				return INSERT_FAILED;
			}
			fprintf(stderr, "Collision detected between : \n %s %s\n", head->current->value, value);
		}
		head = head->next;
	}

	set->keys = realloc(set->keys, (set->number_of_elements + 1) * sizeof(uint64_t));
	assert(set->keys[set->number_of_elements - 1] == UINT64_MAX);
	set->keys[set->number_of_elements - 1] = hashed_value;
	set->keys[set->number_of_elements] = UINT64_MAX;

	assert(head->current->value == NULL);
	assert(head->current->key == UINT64_MAX);
	assert(head->next==NULL);

	head->current->value = strdup(value);

	head->current->key = hashed_value;
	head->next = (list_t *) malloc(sizeof(list_t));
	head->next->current = (element_t *) malloc(sizeof(element_t));
	head->next->current->value = NULL;
	head->next->current->key = UINT64_MAX;
	head->next->next = NULL;

	set->number_of_elements += 1;
	return 0;
}

int remove_set(set_t *set, char *value) {
	if (is_empty_set(set)) {
		return EMPTY_SET;
	}
	uint64_t hashed_value = hash(value);
	list_t *head = set->map->tab[hashed_value % set->map->size];
	int flag = 0;
	while (head->next != NULL) {
		if (head->current->key == hashed_value) {
			if (strcmp(head->current->value, value) == 0) {
				flag = 1;
				free(head->current->value);
				free(head->current);
				head->current = head->next->current;
				list_t * temp=head->next->next;
				free(head->next);
				head->next = temp;
				break;
			} else {
				fprintf(stderr, "Collision detected and avoided\n");
			}
		}
		head=head->next;
	}
	if (!flag) {
		return NOT_IN_SET;
	}
	uint64_t i;
	for (i = 0; i < set->number_of_elements; ++i) {
		if (set->keys[i] == hashed_value) {
			set->keys[i] = set->keys[set->number_of_elements - 2];
			break;
		}
	}
	if (i == set->number_of_elements) {
		return NOT_IN_KEYS;
	}
	set->keys = realloc(set->keys, (set->number_of_elements - 1) * sizeof(uint64_t));
	set->keys[set->number_of_elements - 2] = UINT64_MAX;
	set->number_of_elements -= 1;
	return 0;
}

int contains_set(set_t *set, char *value) {
	if (is_empty_set(set)) {
		return 0;
	}
	uint64_t hashed_value = hash(value);
	list_t *head = set->map->tab[hashed_value % set->map->size];
	int flag = 0;
	while (head->next != NULL) {
		if (head->current->key == hashed_value) {
			if (strcmp(head->current->value, value) == 0) {
				flag = 1;
				break;
			} else {
				fprintf(stderr, "Collision detected and avoided\n");
			}
		}
		head=head->next;
	}
	return flag;
}

char *get_value_set(set_t *set, uint64_t key) {
	list_t *head = set->map->tab[key % set->map->size];
	int flag = 0;
	char *res = NULL;
	while (head->next != NULL) {
		if (head->current->key == key) {
			res = head->current->value;
			flag += 1;
		}
		head = head->next;
	}
	if (flag > 1) {
		fprintf(stderr, "Potential collision detected, result might not be corect\n");
	}
	return res;
}

set_t *union_set(set_t *set1, set_t *set2) {
	set_t *set = create_set();
	for (uint64_t i = 0; i < set1->number_of_elements - 1; ++i) {
		insert_set(set, get_value_set(set1, set1->keys[i]));
	}
	for (uint64_t j = 0; j < set2->number_of_elements - 1; ++j) {
		char *value = get_value_set(set2, set2->keys[j]);
		int res = contains_set(set, value);
		if (res == EMPTY_SET) {
			return set;
		}
		if (!res) {
			insert_set(set, value);
		}
	}
	return set;
}


set_t *union_set_on_condition(set_t *set1, set_t *set2,  int(*condition)(char *)) {
	set_t *set = create_set();
	for (uint64_t i = 0; i < set1->number_of_elements - 1; ++i) {
		if (condition(get_value_set(set1, set1->keys[i]))){
			insert_set(set,get_value_set(set1, set1->keys[i]) );
		}
	}
	for (uint64_t j = 0; j < set2->number_of_elements - 1; ++j) {
		char *value = get_value_set(set2, set2->keys[j]);
		if (condition(value)){
			int res = contains_set(set, value);
			if (res == EMPTY_SET) {
				return set;
			}
			if (!res) {
				insert_set(set, value);
			}
		}
	}
	return set;
}

set_t *substract_set(set_t *set1, set_t *set2) {
	set_t *set = create_set();
	for (uint64_t i = 0; i < set1->number_of_elements - 1; ++i) {
		char *value = get_value_set(set1, set1->keys[i]);
		int res = contains_set(set2, value);
		if (!res) {
			insert_set(set, value);
		}
	}
	return set;
}

set_t *intersect_set(set_t *set1, set_t *set2) {
	set_t *set = create_set();
	for (uint64_t i = 0; i < set1->number_of_elements - 1; ++i) {
		char *value = get_value_set(set1, set1->keys[i]);
		int res = contains_set(set2, value);
		if (res == EMPTY_SET) {
			return set;
		}
		if (res) {
			insert_set(set, value);
		}
	}
	return set;
}

uint64_t get_size_set(set_t *set) {
	return set->number_of_elements;
}

int is_empty_set(set_t *set) {
	assert(set->number_of_elements > 0);
	return set->number_of_elements == 1;
}

uint64_t *get_iterator_set(set_t *set) {
	uint64_t *var=(uint64_t *)malloc(sizeof(uint64_t));
	*var=0;
	return var;
}

char *next_set(set_t *set, uint64_t *index) {
	if ((*index - *set->keys) == set->number_of_elements * sizeof(uint64_t)) {
		return NULL;
	}
	char *res = get_value_set(set, set->keys[*index]);
	*index=(*index)+1;
	return res;
}

