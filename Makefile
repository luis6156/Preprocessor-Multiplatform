COMPILER = cl
CFLAGS = /MD /W3 /D_CRT_SECURE_NO_WARNINGS 

#.PHONY: clean

all: build

run: build
	./so-cpp.exe

build: LinkedList.obj HashTable.obj Utils.obj Preprocessor.obj so-cpp.obj
	$(COMPILER) $** /Feso-cpp.exe

HashTable.o: HashTable.c
	$(COMPILER) $(CFLAGS) /c $**

LinkedList.obj: LinkedList.c
	$(COMPILER) $(CFLAGS) /c $**

so-cpp.obj: so-cpp.c
	$(COMPILER) $(CFLAGS) /c $**

Utils.obj: Utils.c
	$(COMPILER) $(CFLAGS) /c $**

Preprocessor.obj: Preprocessor.c
	$(COMPILER) $(CFLAGS) /c $**

clean: exe_clean obj_clean

obj_clean :
  del *.obj
 
exe_clean :
  del so-cpp.exe
