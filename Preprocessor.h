#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "HashTable.h"

#define CAPACITY_HT 100
#define CAPACITY_DIRECTORIES 5
#define BUFF_SIZE 256
#define DELIM "\t\n\r []{}<>=+-*/%!&|^.,:;()\\"

void format_multiple_spaces(char *str);

void remove_trailing_spaces(char *str);

void add_argument_type_macro(HashTable *ht, char *start_idx);

void add_argument_type_IO(char **input_file, char **output_file,
			  char **parent_directory, char *str);

void create_fd_IO(FILE **in, FILE **out, char *input_file, char *output_file,
		  char **parent_directory);

void initialize_program(int argc, char *argv[], HashTable *ht,
			LinkedList *directories, FILE **in, FILE **out,
			char **parent_directory);

char *compute_macro(char *key, HashTable *ht);

void preprocess_word(char *word, unsigned char is_between_quotes,
		     unsigned int *line_idx, char **line, HashTable *ht);

void preprocess_text(char buffer_copy[BUFF_SIZE], HashTable *ht, FILE *out);

void define_macro(char *key, char *value, FILE *in, HashTable *ht);

void check_statement(char *value, unsigned char *reached_true_branch);

void get_header_fd(char *value, FILE **include_file, char *parent_directory,
		   LinkedList *directories);

void process_file(char *parent_directory, FILE *in, FILE *out, HashTable *ht,
		  LinkedList *directories);

#endif
