# Copyright Micu Florian-Luis 331CA 2022 - Assignment 1

# Organization

The program mimics the functionality of the GCC preprocessor, as implied by 
the assignment. Moreover, I implemented some extra features to better copy
the preprocessor. 

The assignment is useful for anyone that is trying to understand how a compiler
works, more specifically the preprocessor. Furthermore, after this assignment 
macros and their respective directives and conditions are better understood.

My implementation uses dynamic memory everywhere, therefore the program is
a slower but manages memory better. Otherwise the program is efficient and does
its job effectively (all tests are passed). 

# Implementation

## Flags
All of the flags were implemented and if multiple output files are given, an
exit call will be made to notify the user of the error.

## HashTable + LinkedList
In order to create a preprocessor I has to create a HashTable that could
hold mappings for macros. Moreover, the HashTable required a LinkedList
to create a direct chaining behavior when a collision happens.

### Bonus
The HashMap and LinkedList can hold any type of data and the keys and values
that are used are malloc'd inside the data structures in case that their
respective pointers are changed no problem will occur.

## Define
Each macro is held inside the HashTable alongside its associated key. The macro
could be given from the CLI or could be read from the file itself. If a new
value for the same key is redefined then the old value will be thrown away (just
like GCC does it). Moreover, the "#DEFINE" directive supports multi-line values
and it does not change words that are equal to the macro if they are between
quotation marks (e.g. printf("VAR0: %s", VAR0)).

### Bonus
The "#define" command supports space formatting, just like GCC. The trailing 
spaces are removed and all leading spaces are trimmed to just one space (the 
same happens for spaces in between words IF they are not between quotation 
marks). Furthermore, macros are only allowed to start with alphabetical 
letters or an underscore (to mimic GCC).

## Undefine
To undefine a macro, the program simply removes its entry from the HashTable,
therefore no further appearance will be exchanged.

## If/Elif/Else
For this particular command I assumed that only numerical values will be given
to be evaluated (as seen in the test files), thus anything above "0" is true,
anything below or equal to "0" is false. Moreover, to only choose the correct
branch I used a semaphore variable that will be set to true if the condition
is true or false if the condition is false or another logic branch is reached
so that only the body of the true condition will be printed.

### Bonus
If the values is not numerical, an exit call will be made.

## Ifdef/Ifndef
This command follows the principles of the previous command except it checks
for the macro inside of the HashTable according to the condition. The macro
could be represented by another macro, therefore I tokenize each macro and
search each token in the HashTable to get the final result. The program 
allows as many recursion levels as possible for the case of one or more 
macros inside of another macro.  

## Include
For this case, the program opens a new file descriptor using the included
directories (which are stored as a LinkedList to preserve the order) or
from the current directory (if run from STDIN) or from the directory of
the file that was firstly opened (if the file is given as a parameter).
After the file descriptor is established, the function that preprocesses
all of the code is run again this time in the header file and the outcome
is directly printed to the output file, thus preserving correct order (also
this approach does not use a gigantic buffer that could become a heap
overflow).

### Bonus
Since only headers that are inside of quotation marks are allowed, any other
type of include will be disallowed with an exit call.

Infinite recursion is possible as the program allows as many recursion levels
as possible for any condition (just like for the macro inside of a macro case).

## Processing text
Each line of text is tokenized and verified if any macro is present in it so
that it could be exchanged. A copy of the buffer that holds the line is done 
before doing the tokenize operation to get the delimiters so that the output
is correct.

### Safety
Each allocation of dynamic memory is verified, however I could not use the
"DIE" macro as the checker would not allow it. I also used strtok_r to
allow recursion but I needed an implementation.

### Note
I only put comments in the code for code that is not self-explanatory.

# Bibliography
HashTable/LinkedList: https://ocw.cs.pub.ro/sd-ca/
strtok_r: https://code.woboq.org/userspace/glibc/string/strtok_r.c.html
