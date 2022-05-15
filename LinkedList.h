#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
	void *data;
	struct Node *next;
} Node;

typedef struct LinkedList {
	struct Node *head;
	struct Node *tail;
	unsigned int size;
	int (*compare_function)(void *, void *);
	void (*print_function)(void *);
	void (*free_data)();
} LinkedList;

int has_node_list(LinkedList *list, void *data);

void print_strings(void *data);

void print_ints(void *data);

void print_list(LinkedList *list);

void add_first_node_list(LinkedList *list, void *data, unsigned int data_size);

void add_last_node_list(LinkedList *list, void *data, unsigned int data_size);

int get_size_list(LinkedList *list);

int is_empty_list(LinkedList *list);

Node *get_node_list(LinkedList *list, void *data);

void remove_node_list(LinkedList *list, void *data);

void free_list(LinkedList **list);

LinkedList *initialize_list(int (*compare_function)(void *, void *),
			    void (*print_function)(void *),
			    void (*free_data)(void *));

#endif
