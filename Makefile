INCDIR = include
SRCDIR = src
BUILDDIR = build
OBJ = $(wildcard $(BUILDDIR)/*.o)

INC = -I $(INCDIR)
LIBFLAGS = -rdynamic -ldl
CC = gcc
ifeq ($(DEBUG),1)
CFLAGS = -g -Wall -DDEBUG=1 -pthread
else
CFLAGS = -g -Wall -DDEBUG=0 -pthread
endif

PGM = ptar
TARGET = ./$(PGM)

$(shell mkdir -p $(BUILDDIR))


all: build


# Generic task
%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< $(LIBFLAGS) -o $(BUILDDIR)/$@


main: Option.o header_posix_ustar.o utils.o ptar.o main.o w_info.o extract.o
	$(CC) $(CFLAGS) $(OBJ) $(LIBFLAGS) -o $(TARGET)


build: main

clean: deleteFiles
	@rm -rf $(TARGET)
	@rm -rf $(BUILDDIR)


valgrind: build
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all -v $(TARGET) -x archive.tar


### --------------------------------
###  Create tar archive for tests
### --------------------------------
TAR = archive.tar
FILES = link1 file1 file2  test/

deleteFiles:
	@rm -rf $(FILES)
	@rm -rf $(TAR)

createFiles: deleteFiles
	@echo "my first file!" > file1
	@echo "my second file!" > file2
	@ln -s file1 link1
	@mkdir ./test
	@echo "lol" > ./test/file

createArchive: createFiles
	@echo "# Create archive for test"
	tar --format=ustar -cf $(TAR) $(FILES)
	@echo
	@rm -r $(FILES)