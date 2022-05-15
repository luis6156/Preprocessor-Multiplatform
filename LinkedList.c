#include "LinkedList.h"

void add_first_node_list(LinkedList *list, void *data, unsigned int data_size)
{
	Node *new_node;

	if (data == NULL || list == NULL)
		return;

	new_node = malloc(sizeof(*new_node));

	if (!new_node)
		exit(12);
	new_node->data = malloc(data_size);

	if (!new_node->data)
		exit(12);
	memcpy(new_node->data, data, data_size);

	if (is_empty_list(list)) {
		new_node->next = NULL;
		list->head = new_node;
		list->tail = new_node;
	} else {
		new_node->next = list->head;
		list->head = new_node;
	}

	list->size++;
}

void add_last_node_list(LinkedList *list, void *data, unsigned int data_size)
{
	Node *new_node;

	if (data == NULL || list == NULL)
		return;

	new_node = malloc(sizeof(*new_node));

	if (!new_node)
		exit(12);
	new_node->data = malloc(data_size);

	if (!new_node->data)
		exit(12);
	memcpy(new_node->data, data, data_size);
	new_node->next = NULL;

	if (is_empty_list(list)) {
		list->head = new_node;
		list->tail = new_node;
	} else {
		list->tail->next = new_node;
		list->tail = new_node;
	}

	list->size++;
}

int get_size_list(LinkedList *list)
{
	if (list == NULL)
		return -1;

	return list->size;
}

int is_empty_list(LinkedList *list)
{
	if (list == NULL)
		return -1;

	return get_size_list(list) == 0;
}

int has_node_list(LinkedList *list, void *data)
{
	Node *curr;

	if (data == NULL || list == NULL)
		return -1;

	curr = list->head;

	while (curr != NULL) {
		if (!list->compare_function(curr->data, data))
			return 1;
		curr = curr->next;
	}

	return 0;
}

Node *get_node_list(LinkedList *list, void *data)
{
	Node *curr;

	if (data == NULL || list == NULL)
		return NULL;

	curr = list->head;

	while (curr != NULL) {
		if (!list->compare_function(curr->data, data))
			return curr;

		curr = curr->next;
	}

	return NULL;
}

void remove_node_list(LinkedList *list, void *data)
{
	Node *prev, *curr;

	if (data == NULL || list == NULL || is_empty_list(list))
		return;

	if (get_size_list(list) == 1) {
		list->free_data(list->head->data);
		free(list->head);
		list->head = NULL;
		list->tail = NULL;
		list->size--;
		return;
	}

	if (!list->compare_function(list->head->data, data)) {
		Node *tmp = list->head;

		list->head = list->head->next;
		list->size--;
		list->free_data(tmp->data);
		free(tmp);
		return;
	}

	prev = list->head;
	curr = list->head->next;
	while (curr != list->tail) {
		if (!list->compare_function(curr->data, data)) {
			prev->next = curr->next;
			list->size--;
			free(curr->data);
			free(curr);
			return;
		}
		prev = curr;
		curr = curr->next;
	}

	if (!list->compare_function(list->tail->data, data)) {
		Node *tmp = list->tail;

		prev->next = NULL;
		list->tail = prev;
		list->size--;
		list->free_data(tmp->data);
		free(tmp);
	}
}

void print_list(LinkedList *list)
{
	Node *curr = list->head;

	while (curr != NULL) {
		list->print_function(curr->data);
		curr = curr->next;
	}

	printf("\n");
}

void print_ints(void *data) { printf("%d ", *(int *)data); }

void print_strings(void *data) { printf("%s ", (char *)data); }

void free_list(LinkedList **list)
{
	Node *curr = (*list)->head;

	while (curr != NULL) {
		Node *tmp = curr;

		curr = curr->next;
		(*list)->free_data(tmp->data);
		free(tmp);
	}

	free(*list);
}

LinkedList *initialize_list(int (*compare_function)(void *, void *),
			    void (*print_function)(void *),
			    void (*free_data)(void *))
{
	LinkedList *list = calloc(1, sizeof(*list));

	if (!list)
		exit(12);

	list->compare_function = compare_function;
	list->print_function = print_function;
	list->free_data = free_data;

	return list;
}
