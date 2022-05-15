COMPILER = gcc
CFLAGS = -Wall -Wextra -Wno-unused-variable -g

.PHONY: clean

all: build

run: build
	./so-cpp

build: LinkedList.o HashTable.o Utils.o Preprocessor.o so-cpp.o
	$(COMPILER) $^ -o so-cpp

HashTable.o: HashTable.c
	$(COMPILER) $(CFLAGS) -c $^

LinkedList.o: LinkedList.c
	$(COMPILER) $(CFLAGS) -c $^

so-cpp.o: so-cpp.c
	$(COMPILER) $(CFLAGS) -c $^

Utils.o: Utils.c
	$(COMPILER) $(CFLAGS) -c $^

Preprocessor.o: Preprocessor.c
	$(COMPILER) $(CFLAGS) -c $^

clean:
	rm -rf *.o
	rm -rf so-cpp
