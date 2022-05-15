#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "LinkedList.h"

typedef struct HashTable {
	LinkedList **lists;
	unsigned int size; // DE UPDATAT
	unsigned int capacity;
	unsigned int (*hash_function)(void *f);
	int (*compare_function)(void *f1, void *f2);
} HashTable;

typedef struct Entry {
	void *key;
	void *value;
} Entry;

void print_hashtable(HashTable *ht);

int has_value_hashtable(void *key, HashTable *ht);

void free_entries(void *data);

void print_function_entries(void *data);

unsigned int hash_function_strings(void *a);

unsigned int hash_function_int(void *a);

int compare_strings(void *a, void *b);

int compare_ints(void *a, void *b);

int compare_keys_int(void *a, void *b);

int compare_keys_string(void *a, void *b);

HashTable *initialize_hashtable(unsigned int capacity,
				unsigned int (*hash_function)(void *),
				int (*compare_function)(void *, void *));

void put_hashtable(void *key, unsigned int key_size, void *value,
		   unsigned int value_size, HashTable *ht);

void remove_entry_hashtable(void *key, HashTable *ht);

void *get_value_hashtable(void *key, HashTable *ht);

void free_hashtable(HashTable **ht);

#endif
