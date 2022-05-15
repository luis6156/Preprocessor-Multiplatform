#include "Utils.h"

/*
 * Just like realloc except it does calloc on empty pointer.
 */
void *my_realloc(void *__ptr, size_t __size)
{
	void *new_ptr;

	if (!__ptr) {
		void *tmp = malloc(__size);

		if (!tmp)
			exit(12);
		memset(tmp, '\0', __size);

		return tmp;
	}

	new_ptr = realloc(__ptr, __size);

	if (!new_ptr)
		exit(12);

	return new_ptr;
}

/*
 * Reentrant strtok for Windows/Linux
 */
char *my_strtok_r(char *s, const char *delim, char **save_ptr)
{
	/*
	 * Credits:
	 * https://code.woboq.org/userspace/glibc/string/strtok_r.c.html
	 */
	char *end;

	if (s == NULL)
		s = *save_ptr;

	if (*s == '\0') {
		*save_ptr = s;
		return NULL;
	}

	/* Scan leading delimiters.  */
	s += strspn(s, delim);

	if (*s == '\0') {
		*save_ptr = s;
		return NULL;
	}

	/* Find the end of the token.  */
	end = s + strcspn(s, delim);

	if (*end == '\0') {
		*save_ptr = end;
		return s;
	}

	/* Terminate the token and make *SAVE_PTR point past it.  */
	*end = '\0';
	*save_ptr = end + 1;

	return s;
}
