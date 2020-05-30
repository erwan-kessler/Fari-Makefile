#define SNOW_ENABLED

#include "../src/Set.h"
#include "../snow/snow.h"

static char *rand_string(char *str, size_t size) {
	const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
	if (size) {
		--size;
		for (size_t n = 0; n < size; n++) {
			int key = rand() % (int) (sizeof charset - 1);
			str[n] = charset[key];
		}
		str[size] = '\0';
	}
	return str;
}

describe(Set_Tests) {
	it("Creation of set") {
		set_t *set = create_set();
		asserteq_int(set->number_of_elements, 1, "The number of elements in the set was not correctly initialized");
		asserteq_int(set->keys[0], UINT64_MAX, "The first element of the array is not correctly initialized");
		asserteq_int(set->map->size, HEAP_SIZE, "The hashmap init size is not correct");
		for (int i = 0; i < HEAP_SIZE; ++i) {
			assertneq(set->map->tab[i]->current, NULL, "The element was not correctly initialized");
			asserteq_int(set->map->tab[i]->current->key, UINT64_MAX, "An element key was not correctly initialized");
			asserteq(set->map->tab[i]->current->value, NULL, "An element value was not correctly initialized");
			asserteq(set->map->tab[i]->next, NULL, "The list chain was not correctly initialized");
		}
		delete_set(set);
	}

	it("Empty set test") {
		set_t *set = create_set();
		asserteq_int(is_empty_set(set), 1, "The set was declared as not empty but is");
		delete_set(set);
	}

	it("Insert element in Test") {
		set_t *set = create_set();
		char *value = "hey";
		int res = insert_set(set, value);
		asserteq_int(res, 0, "The function returned an error code but shouldn't");
		asserteq_int(is_empty_set(set), 0, "The set was declared as empty but is filled with one element");
		asserteq_int(set->number_of_elements, 2, "The number of elements in the set was not updated");
		asserteq_int(set->keys[0], hash(value), "The first key was not updated accordingly");
		asserteq_int(set->keys[1], UINT64_MAX, "The last key was not updated accordingly");
		asserteq_str(set->map->tab[hash(value) % HEAP_SIZE]->current->value, value, "The last key was not updated accordingly");

		res = insert_set(set, value);
		asserteq_int(res, -6, "The function returned something other than INSERT_FAILED but shouldn't");
		asserteq_int(set->number_of_elements, 2, "The number of elements in the set was not updated");
		asserteq_int(set->keys[0], hash(value), "The first key was not updated accordingly");
		asserteq_int(set->keys[1], UINT64_MAX, "The last key was not updated accordingly");
		delete_set(set);

	}

	it("Insert multiple elements in Test (hashmap resizing)") {
		char *value=malloc(sizeof(char) * 30);
		set_t *set = create_set();
		for (int i = 0; i < HEAP_SIZE * 2 / 3 - 1; ++i) {
			value = rand_string(value, 30);
			insert_set(set, value);
		}
		asserteq_int(set->number_of_elements, 43690, "The number of elements in the set was not updated");
		asserteq_int(set->map->size, 65535, "The hashmap was resized whereas it shouldn't have");
		insert_set(set, rand_string(value,30));
		asserteq_int(set->map->size, 131070, "The hashmap was not resized whereas it should have");
		asserteq_int(set->number_of_elements, 43691, "The number of elements in the set was not updated");
		delete_set(set);
        free(value);
	}

	it("Delete element") {
		set_t *set = create_set();
		char *value = "hey";
		insert_set(set, value);
		insert_set(set, "ho");
		asserteq_int(set->keys[2], UINT64_MAX, "The first key was not updated accordingly");
		int res = remove_set(set, value);
		asserteq_int(res, 0, "The function returned an error code");
		asserteq_int(set->number_of_elements, 2, "The number of elements in the set was not updated");
		asserteq_int(set->keys[0], hash("ho"), "The first key was not updated accordingly");
		asserteq_int(set->keys[1], UINT64_MAX, "The second key was not updated accordingly");
		res = remove_set(set, value);
		asserteq_int(res, -1, "The element was actually removed whereas it shouldn't");
		res = remove_set(set, "ho");
		asserteq_int(res, 0, "The removal did not succeeded");
		asserteq_int(is_empty_set(set), 1, "The set is not empty");
		delete_set(set);
	}

	it("Contains element") {
		set_t *set = create_set();
		char *value = "hey";
		insert_set(set, value);
		insert_set(set, "ho");
		asserteq_int(contains_set(set, value), 1, "The value was not found in the set");
		remove_set(set, value);
		asserteq_int(contains_set(set, value), 0, "The value was found in the set");
		asserteq_int(contains_set(set, "ho"), 1, "The value was not found in the set");
		delete_set(set);
	}

	it("Contains element with resize") {
		char *value= malloc(sizeof(char) * 30);
		set_t *set = create_set();
		for (int i = 0; i < HEAP_SIZE * 2 / 3 - 1; ++i) {
			value = rand_string(value, 30);
			insert_set(set, value);
		}
		value = rand_string(value, 30);
		asserteq_int(contains_set(set, value), 0, "The value was found in the set");
		insert_set(set, value);
		asserteq_int(contains_set(set, value), 1, "The value was not found in the set");
		remove_set(set, value);
		asserteq_int(contains_set(set, value), 0, "The value was found in the set");
		delete_set(set);
		free(value);
	}

	it("Union of sets") {
		set_t *set1 = create_set();
		set_t *set2 = create_set();
		insert_set(set1, "ho");
		asserteq_int(get_size_set(set1), 2, "The set doesnt have the correct size");
		insert_set(set2, "ho");
		asserteq_int(get_size_set(set2), 2, "The set doesnt have the correct size");
		set_t *set = union_set(set1, set2);
		asserteq_int(contains_set(set, "ho"), 1, "The value was found in the set");
		asserteq_int(get_size_set(set), 2, "The set doesnt have the correct size");
		insert_set(set1,"hey");
		set_t *set3=union_set(set1,set);
		asserteq_int(contains_set(set3, "hey"), 1, "The value was found in the set");
		asserteq_int(get_size_set(set3), 3, "The set doesnt have the correct size");
		delete_set(set);
		delete_set(set1);
		delete_set(set2);
		delete_set(set3);
	}

	it("Intersection of sets") {
		set_t *set1 = create_set();
		set_t *set2 = create_set();
		insert_set(set1, "ho");
		asserteq_int(get_size_set(set1), 2, "The set doesnt have the correct size");
		insert_set(set2, "ho");
		asserteq_int(get_size_set(set2), 2, "The set doesnt have the correct size");
		set_t *set = intersect_set(set1, set2);
		asserteq_int(contains_set(set, "ho"), 1, "The value was found in the set");
		asserteq_int(get_size_set(set), 2, "The set doesnt have the correct size");
		insert_set(set1,"hey");
		set_t *set3=intersect_set(set1,set);
		asserteq_int(contains_set(set3, "hey"), 0, "The value was found in the set");
		asserteq_int(get_size_set(set3), 2, "The set doesnt have the correct size");
		delete_set(set);
		delete_set(set1);
		delete_set(set2);
		delete_set(set3);
	}

	it("Substraction of sets") {
		set_t *set1 = create_set();
		set_t *set2 = create_set();
		insert_set(set1, "ho");
		asserteq_int(get_size_set(set1), 2, "The set doesnt have the correct size");
		insert_set(set2, "ho");
		asserteq_int(get_size_set(set2), 2, "The set doesnt have the correct size");
		set_t *set = substract_set(set1, set2);
		asserteq_int(contains_set(set, "ho"), 0, "The value was found in the set");
		asserteq_int(get_size_set(set), 1, "The set doesnt have the correct size");
		insert_set(set1,"hey");
		set_t *set3=substract_set(set1,set2);
		asserteq_int(contains_set(set3, "hey"), 1, "The value was found in the set");
		asserteq_int(get_size_set(set3), 2, "The set doesnt have the correct size");
		delete_set(set);
		delete_set(set1);
		delete_set(set2);
		delete_set(set3);
	}

	it("Iteration over sets") {
		char*value;
		set_t *set1 = create_set();
		insert_set(set1, "ho");
		insert_set(set1, "hey");
		asserteq_int(get_size_set(set1), 3, "The set doesnt have the correct size");
		uint64_t *ptr=get_iterator_set(set1);
		asserteq_int(*ptr,0,"The iterator given was not correct");
		value=next_set(set1,ptr);
		asserteq_int(*ptr,1,"The iterator given was not correct");
		asserteq_str(value,"ho","The value given by the iterator was incorrect");
		value=next_set(set1,ptr);
		asserteq_str(value,"hey","The value given by the iterator was incorrect");
		asserteq_int(*ptr,2,"The iterator given was not correct");
		value=next_set(set1,ptr);
		asserteq(value,NULL,"The iterator did not terminate well");
		delete_set(set1);
		free(ptr);
	}
}

snow_main();
