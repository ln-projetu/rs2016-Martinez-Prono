INCDIR = include
SRCDIR = src
BUILDDIR = build
OBJ = $(wildcard $(BUILDDIR)/*.o)

INC = -I $(INCDIR)
CC = gcc
CFLAGS = -g -Wall -DDEBUG=0

PGM = ptar
TARGET = ./$(PGM)

$(shell mkdir -p $(BUILDDIR))
$(shell mkdir -p bin)


all: createArchive build


# Generic task
%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $(BUILDDIR)/$@


main: Option.o block.o header_posix_ustar.o utils.o ptar.o main.o
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)


build: main


valgrind: build
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all -v $(TARGET) archive.tar


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
