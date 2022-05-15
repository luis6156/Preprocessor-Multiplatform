#include "Preprocessor.h"

int main(int argc, char *argv[])
{
	char *token = NULL, *parent_directory;
	FILE *in = NULL, *out = NULL;

	HashTable *ht = initialize_hashtable(CAPACITY_HT, hash_function_strings,
					     compare_keys_string);
	LinkedList *directories =
	    initialize_list(compare_strings, print_strings, free);

	initialize_program(argc, argv, ht, directories, &in, &out,
			   &parent_directory);

	process_file(parent_directory, in, out, ht, directories);

	fclose(in);
	fclose(out);
	free(parent_directory);
	free_hashtable(&ht);
	free_list(&directories);

	return 0;
}
