#include "Preprocessor.h"

/*
 * Removes any spaces from the start of the string to the end of it.
 */
void format_multiple_spaces(char *str)
{
	char *dest = str;

	while (*str != '\0') {
		while ((*str == ' ' && *(str + 1) == ' ') ||
		       (*str == '\t' && *(str + 1) == '\t') ||
		       (*str == ' ' && *(str + 1) == '\t') ||
		       (*str == '\t' && *(str + 1) == ' ')) {
			++str;
		}

		*dest++ = *str++;
	}

	*dest = '\0';
}

/*
 * Removes any spaces from the end of the string.
 */
void remove_trailing_spaces(char *str)
{
	int i = strlen(str) - 1;

	while (i >= 0 && (str[i] == ' ' || str[i] == '\t')) {
		str[i] = '\0';
		--i;
	}
}

/*
 * Puts inside the HashTable the macro defined from the CLI or "" if empty.
 */
void add_argument_type_macro(HashTable *ht, char *start_idx)
{
	char *key = strtok(start_idx, "\n\r= ");
	char *value = strtok(NULL, "\n\r= ");

	if (value == NULL)
		put_hashtable(key, strlen(key) + 1, "", 1, ht);
	else
		put_hashtable(key, strlen(key) + 1, value, strlen(value) + 1,
			      ht);
}

/*
 * Checks if the input and output file have been established and sets them
 * accordingly.
 */
void add_argument_type_IO(char **input_file, char **output_file,
			  char **parent_directory, char *str)
{
	if (*input_file == NULL) {
		// First file given is always input
		char *last_slash;

		*input_file = calloc(strlen(str) + 1, sizeof(char));

		if (*input_file == NULL)
			exit(12);

		memcpy(*input_file, str, strlen(str) + 1);

		// Create the parent directory for it
		last_slash = strrchr(*input_file, '/');
		if (last_slash == NULL) {
			*parent_directory =
			    calloc(strlen("./") + 1, sizeof(char));

			if (*parent_directory == NULL)
				exit(12);
			memcpy(*parent_directory, "./", 2);
		} else {
			int length =
			    strlen(*input_file) - strlen(last_slash) + 1;
			*parent_directory = calloc(length + 1, sizeof(char));

			if (*parent_directory == NULL)
				exit(12);
			memcpy(*parent_directory, *input_file, length);
		}
	} else if (*output_file == NULL) {
		// Second, the output file is given
		*output_file = calloc(strlen(str) + 1, sizeof(char));

		if (*output_file == NULL)
			exit(12);
		memcpy(*output_file, str, strlen(str) + 1);
	} else {
		// Any other file is extra.
		exit(12);
	}
}

/*
 * Creates the file descriptor for input and output.
 */
void create_fd_IO(FILE **in, FILE **out, char *input_file, char *output_file,
		  char **parent_directory)
{
	if (input_file != NULL) {
		*in = fopen(input_file, "r");
	} else {
		*in = stdin;
		*parent_directory = calloc(strlen("./") + 1, sizeof(char));

		if (*parent_directory == NULL)
			exit(12);
		memcpy(*parent_directory, "./", 2);
	}

	if (*in == NULL)
		exit(12);

	if (output_file != NULL)
		*out = fopen(output_file, "w");
	else
		*out = stdout;

	if (*out == NULL)
		exit(12);
}

/*
 * Initialize the program with all of the flags given.
 */
void initialize_program(int argc, char *argv[], HashTable *ht,
			LinkedList *directories, FILE **in, FILE **out,
			char **parent_directory)
{
	char *input_file = NULL, *output_file = NULL;
	int i;

	for (i = 1; i < argc; ++i) {
		if (!strncmp(argv[i], "-D", 2)) {
			// define macros
			char *start_idx;

			if (strlen(argv[i]) > 2) {
				start_idx = argv[i] + 2;
			} else {
				++i;
				start_idx = argv[i];
			}

			add_argument_type_macro(ht, start_idx);
		} else if (!strncmp(argv[i], "-I", 2)) {
			// define directories
			char *start_idx;

			if (strlen(argv[i]) > 2) {
				start_idx = argv[i] + 2;
			} else {
				++i;
				start_idx = argv[i];
			}

			add_last_node_list(directories, start_idx,
					   strlen(start_idx) + 1);
		} else if (!strncmp(argv[i], "-o", 2)) {
			// define output
			char *start_idx;

			if (strlen(argv[i]) > 2) {
				start_idx = argv[i] + 2;
			} else {
				++i;
				start_idx = argv[i];
			}

			output_file =
			    calloc(strlen(start_idx) + 1, sizeof(char));

			if (output_file == NULL)
				exit(12);
			memcpy(output_file, start_idx, strlen(start_idx) + 1);
		} else if (!strncmp(argv[i], "-", 1)) {
			// wrong argument
			exit(12);
		} else {
			// input/output
			add_argument_type_IO(&input_file, &output_file,
					     parent_directory, argv[i]);
		}
	}

	// create FD for input/output
	create_fd_IO(in, out, input_file, output_file, parent_directory);

	free(input_file);
	free(output_file);
}

/*
 * Gets the value of the macro recursively by tokenizing it
 * (macro inside macro).
 */
char *compute_macro(char *key, HashTable *ht)
{
	char *multiple_values = NULL, *save_ptr = NULL, *value_copy = NULL,
	     *token = NULL;
	unsigned int index = 0, size = 1;
	char *value = get_value_hashtable(key, ht);

	// No entry
	if (value == NULL)
		return NULL;

	value_copy = calloc(strlen(value) + 1, sizeof(char));

	if (!value_copy)
		exit(12);
	memcpy(value_copy, value, strlen(value) + 1);

	// Inside strings, return value
	if (value[0] == '\"' && value[strlen(value) - 1] == '\"')
		return value_copy;

	token = my_strtok_r(value_copy, DELIM, &save_ptr);

	if (token == NULL) {
		multiple_values = calloc(strlen(value) + 1, sizeof(char));

		if (!multiple_values)
			exit(12);
	}

	// tokenize the macro into multiple words and call this function again
	while (token != NULL) {
		unsigned int end_idx;
		unsigned int start_idx = token - value_copy + strlen(token);
		char *transformed_value = compute_macro(token, ht);

		if (transformed_value != NULL) {
			size += strlen(transformed_value);
			multiple_values =
			    my_realloc(multiple_values, size * sizeof(char));

			if (!multiple_values)
				exit(12);
			strcat(multiple_values, transformed_value);
			free(transformed_value);
		} else {
			size += strlen(token);
			multiple_values =
			    my_realloc(multiple_values, size * sizeof(char));

			if (!multiple_values)
				exit(12);
			strcat(multiple_values, token);
		}
		multiple_values[size - 1] = '\0';
		token = my_strtok_r(NULL, DELIM, &save_ptr);
		end_idx = token != NULL ? (unsigned int)(token - value_copy)
					: strlen(value);

		if (end_idx - start_idx > 0) {
			size += (end_idx - start_idx);
			multiple_values =
			    realloc(multiple_values, size * sizeof(char));

			if (!multiple_values)
				exit(12);
			strncat(multiple_values, value + start_idx,
				end_idx - start_idx);
			multiple_values[size - 1] = '\0';
		}
	}

	free(value_copy);

	return multiple_values;
}

/*
 * Preprocesses a word read from a file (exchange macro or print directly).
 */
void preprocess_word(char *word, unsigned char is_between_quotes,
		     unsigned int *line_idx, char **line, HashTable *ht)
{
	if (is_between_quotes) {
		*line_idx += strlen(word);
		*line = my_realloc(*line, *line_idx * sizeof(char));

		if (*line == NULL)
			exit(12);
		strcat(*line, word);
	} else {
		char *value = compute_macro(word, ht);

		if (value != NULL) {
			*line_idx += strlen(value);
			*line = my_realloc(*line, *line_idx * sizeof(char));

			if (*line == NULL)
				exit(12);
			strcat(*line, value);
		} else {
			*line_idx += strlen(word);
			*line = my_realloc(*line, *line_idx * sizeof(char));

			if (*line == NULL)
				exit(12);
			strcat(*line, word);
		}
		free(value);
	}
	(*line)[*line_idx - 1] = '\0';
}

/*
 * Preprocesses the line of a file.
 */
void preprocess_text(char buffer_copy[BUFF_SIZE], HashTable *ht, FILE *out)
{
	char word[BUFF_SIZE] = {0};
	unsigned int i, j = 0, line_idx = 1;
	char *line = NULL;
	unsigned char is_between_quotes = 0;

	for (i = 0; i < strlen(buffer_copy); ++i) {
		if (strchr(DELIM, buffer_copy[i]) == NULL) {
			if (buffer_copy[i] == '\"')
				is_between_quotes = !is_between_quotes;

			word[j] = buffer_copy[i];
			++j;
		} else {
			if (strlen(word) > 0)
				preprocess_word(word, is_between_quotes,
						&line_idx, &line, ht);

			++line_idx;
			line = my_realloc(line, line_idx * sizeof(char));

			if (line == NULL)
				exit(12);
			line[line_idx - 2] = buffer_copy[i];
			j = 0;
			memset(word, '\0', BUFF_SIZE);
			line[line_idx - 1] = '\0';
		}
	}

	if (strlen(word) > 0)
		preprocess_word(word, is_between_quotes, &line_idx, &line, ht);

	fprintf(out, "%s", line);
	free(line);
}

/*
 * Adds a macro to the HashTable.
 */
void define_macro(char *key, char *value, FILE *in, HashTable *ht)
{
	char buffer[BUFF_SIZE];
	char *token;

	// No value
	if (value == NULL) {
		put_hashtable(key, strlen(key) + 1, "", 1, ht);
		return;
	}

	if (value[strlen(value) - 1] == '\\') {
		// Multi-line macro
		char *multiline_value;
		char *key_copy = calloc(strlen(key) + 1, sizeof(char));

		if (!key_copy)
			exit(12);

		memcpy(key_copy, key, strlen(key) + 1);
		value[strlen(value) - 1] = '\0';
		remove_trailing_spaces(value);
		multiline_value = calloc(strlen(value) + 1, sizeof(char));

		if (!multiline_value)
			exit(12);
		memcpy(multiline_value, value, strlen(value) + 1);

		while (fgets(buffer, BUFF_SIZE, in)) {
			token = strtok(buffer, "\n\r");
			format_multiple_spaces(token);

			if (token[strlen(token) - 1] == '\\') {
				token[strlen(token) - 1] = '\0';
				remove_trailing_spaces(token);
				multiline_value = realloc(
				    multiline_value, strlen(multiline_value) +
							 strlen(token) + 1);

				if (!multiline_value)
					exit(12);
				strcat(multiline_value, token);
			} else {
				remove_trailing_spaces(token);
				multiline_value = realloc(
				    multiline_value, strlen(multiline_value) +
							 strlen(token) + 1);

				if (!multiline_value)
					exit(12);
				strcat(multiline_value, token);
				put_hashtable(key_copy, strlen(key_copy) + 1,
					      multiline_value,
					      strlen(multiline_value) + 1, ht);
				break;
			}
		}

		free(key_copy);
		free(multiline_value);
		return;
	}

	// Store the value
	put_hashtable(key, strlen(key) + 1, value, strlen(value) + 1, ht);
}

/*
 * Checks an IF/ELIF statemnt truth.
 */
void check_statement(char *value, unsigned char *reached_true_branch)
{
	int numerical_condition;

	if (value[0] < '0' || value[0] > '9')
		exit(12);

	numerical_condition = atoi(value);

	if (numerical_condition > 0)
		*reached_true_branch = 1;
	else
		*reached_true_branch = 0;
}

/*
 * Gets header full path and FD.
 */
void get_header_fd(char *value, FILE **include_file, char *parent_directory,
		   LinkedList *directories)
{
	char *path;

	// Wrong input
	if (value[0] != '\"' && value[strlen(value) - 1] != '\"')
		exit(12);

	value++;
	value[strlen(value) - 1] = '\0';

	path =
	    calloc(strlen(parent_directory) + strlen(value) + 1, sizeof(char));

	if (!path)
		exit(12);

	// Search in the given path (STDIN/CLI)
	memcpy(path, parent_directory, strlen(parent_directory) + 1);
	strcat(path, value);
	*include_file = fopen(path, "r");
	free(path);

	if (*include_file == NULL && is_empty_list(directories))
		exit(12);

	// Check inside directories
	if (*include_file == NULL && !is_empty_list(directories)) {
		Node *curr = directories->head;

		while (curr != NULL) {
			path = calloc(strlen(curr->data) + strlen(value) + 2,
				      sizeof(char));

			if (!path)
				exit(12);
			memcpy(path, curr->data, strlen(curr->data) + 1);
			strcat(path, "/");
			strcat(path, value);
			*include_file = fopen(path, "r");
			free(path);

			if (*include_file != NULL)
				break;

			curr = curr->next;
		}

		if (*include_file == NULL && is_empty_list(directories))
			exit(12);
	}
}

/*
 * Processes text of a file alongside its macros and conditions
 */
void process_file(char *parent_directory, FILE *in, FILE *out, HashTable *ht,
		  LinkedList *directories)
{
	char buffer[BUFF_SIZE] = {0}, buffer_copy[BUFF_SIZE] = {0};
	char *token, *save_ptr;
	unsigned char reached_true_branch = 1;

	while (fgets(buffer, BUFF_SIZE, in)) {
		if (buffer[0] == '\n')
			continue;

		memcpy(buffer_copy, buffer, BUFF_SIZE);
		token = my_strtok_r(buffer, DELIM, &save_ptr);

		if (token == NULL) {
			preprocess_text(buffer_copy, ht, out);
			continue;
		}

		if (!strcmp(token, "#define")) {
			if (reached_true_branch) {
				char *key = my_strtok_r(NULL, DELIM, &save_ptr);
				char *value =
				    my_strtok_r(NULL, "\n\r", &save_ptr);

				define_macro(key, value, in, ht);
			}
		} else if (!strcmp(token, "#ifdef")) {
			if (reached_true_branch) {
				char *computed_value;
				char *value =
				    my_strtok_r(NULL, DELIM, &save_ptr);

				if (!((value[0] >= 'A' && value[0] <= 'Z') ||
				      (value[0] >= 'a' && value[0] <= 'z') ||
				      value[0] == '_'))
					exit(12);

				computed_value = compute_macro(value, ht);

				if (computed_value != NULL) {
					free(computed_value);
					reached_true_branch = 1;
				} else {
					reached_true_branch = 0;
				}
			}
		} else if (!strcmp(token, "#ifndef")) {
			if (reached_true_branch) {
				char *computed_value;
				char *value =
				    my_strtok_r(NULL, DELIM, &save_ptr);

				if (!((value[0] >= 'A' && value[0] <= 'Z') ||
				      (value[0] >= 'a' && value[0] <= 'z') ||
				      value[0] == '_'))
					exit(12);

				computed_value = compute_macro(value, ht);

				if (computed_value == NULL) {
					reached_true_branch = 1;
				} else {
					free(computed_value);
					reached_true_branch = 0;
				}
			}
		} else if (!strcmp(token, "#if")) {
			char *value = my_strtok_r(NULL, DELIM, &save_ptr);
			char *computed_value = compute_macro(value, ht);

			if (computed_value != NULL)
				value = computed_value;

			check_statement(value, &reached_true_branch);

			if (computed_value != NULL)
				free(computed_value);
		} else if (!strcmp(token, "#elif")) {
			if (reached_true_branch == 0) {
				char *value =
				    my_strtok_r(NULL, DELIM, &save_ptr);
				char *computed_value = compute_macro(value, ht);

				if (computed_value != NULL)
					value = computed_value;

				check_statement(value, &reached_true_branch);

				if (computed_value != NULL)
					free(computed_value);
			} else {
				reached_true_branch = 0;
			}
		} else if (!strcmp(token, "#else")) {
			reached_true_branch = !reached_true_branch;
		} else if (!strcmp(token, "#endif")) {
			reached_true_branch = 1;
		} else if (!strcmp(token, "#undef")) {
			if (reached_true_branch) {
				char *key = my_strtok_r(NULL, DELIM, &save_ptr);

				remove_entry_hashtable(key, ht);
			}
		} else if (!strcmp(token, "#include")) {
			FILE *include_file;
			char *value = my_strtok_r(NULL, "\n\r", &save_ptr);

			get_header_fd(value, &include_file, parent_directory,
				      directories);

			process_file(parent_directory, include_file, out, ht,
				     directories);
			fclose(include_file);
		} else {
			if (reached_true_branch)
				preprocess_text(buffer_copy, ht, out);
		}
	}
}
