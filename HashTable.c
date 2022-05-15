#include "HashTable.h"

#define PRIME 7
#define MAGIC_NUMBER 31

void print_hashtable(HashTable *ht)
{
	unsigned int i;
	for (i = 0; i < ht->capacity; ++i)
		if (!is_empty_list(ht->lists[i]))
			print_list(ht->lists[i]);
}

void print_function_entries(void *data)
{
	print_strings(((Entry *)data)->key);
	print_strings(((Entry *)data)->value);
}

unsigned int hash_function_strings(void *a)
{
	unsigned int i, hash = PRIME;

	for (i = 0; i < strlen((char *)a); ++i)
		hash = hash * MAGIC_NUMBER + ((char *)a)[i];

	return hash;
}

int compare_keys_string(void *a, void *b)
{
	return strcmp((char *)((Entry *)a)->key, (char *)b);
}

int compare_strings(void *a, void *b) { return strcmp((char *)a, (char *)b); }

void free_entries(void *data)
{
	Entry *entry;

	if (data == NULL)
		return;

	entry = (Entry *)data;

	free(entry->key);
	free(entry->value);
	free(entry);
}

HashTable *initialize_hashtable(unsigned int capacity,
				unsigned int (*hash_function)(void *),
				int (*compare_function)(void *, void *))
{
	unsigned int i;
	HashTable *ht = (HashTable *)calloc(1, sizeof(HashTable));

	if (!ht)
		exit(12);

	ht->lists = (LinkedList **)calloc(capacity, sizeof(LinkedList *));

	if (!ht->lists)
		exit(12);

	for (i = 0; i < capacity; ++i)
		ht->lists[i] = initialize_list(
		    compare_function, print_function_entries, free_entries);

	ht->capacity = capacity;
	ht->hash_function = hash_function;
	ht->compare_function = compare_function;

	return ht;
}

void put_hashtable(void *key, unsigned int key_size, void *value,
		   unsigned int value_size, HashTable *ht)
{
	Entry entry, *tmp;
	unsigned int index;

	if (key == NULL || value == NULL || ht == NULL)
		return;

	index = ht->hash_function(key) % ht->capacity;

	tmp = get_value_hashtable(key, ht);
	if (tmp != NULL) {
		tmp->value = realloc(tmp->value, value_size);

		if (!tmp->value)
			exit(12);
		memcpy(tmp->value, value, value_size);
		return;
	}

	entry.key = malloc(key_size);

	if (!entry.key)
		exit(12);
	memcpy(entry.key, key, key_size);

	entry.value = malloc(value_size);

	if (!entry.value)
		exit(12);
	memcpy(entry.value, value, value_size);

	add_first_node_list(ht->lists[index], &entry, sizeof(entry));
}

void remove_entry_hashtable(void *key, HashTable *ht)
{
	unsigned int index = ht->hash_function(key) % ht->capacity;

	remove_node_list(ht->lists[index], key);
}

int has_value_hashtable(void *key, HashTable *ht)
{
	unsigned int index;
	Node *curr;

	if (key == NULL || ht == NULL)
		return -1;

	index = ht->hash_function(key) % ht->capacity;
	curr = ht->lists[index]->head;

	while (curr != NULL) {
		if (!ht->compare_function(curr->data, key))
			return 1;
		curr = curr->next;
	}

	return 0;
}

void *get_value_hashtable(void *key, HashTable *ht)
{
	unsigned int index;
	Node *curr;

	if (key == NULL || ht == NULL)
		return NULL;

	index = ht->hash_function(key) % ht->capacity;
	curr = ht->lists[index]->head;

	while (curr != NULL) {
		if (!ht->compare_function(curr->data, key))
			return ((Entry *)curr->data)->value;
		curr = curr->next;
	}

	return NULL;
}

void free_hashtable(HashTable **ht)
{
	unsigned int i;

	if (ht == NULL || *ht == NULL)
		return;

	for (i = 0; i < (*ht)->capacity; ++i)
		free_list(&(*ht)->lists[i]);

	free((*ht)->lists);
	free(*ht);
}
