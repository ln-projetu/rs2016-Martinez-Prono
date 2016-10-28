INCDIR = include
SRCDIR = src
BUILDDIR = build
OBJ = $(wildcard $(BUILDDIR)/*.o)

INC = -I $(INCDIR)
CC = gcc
CFLAGS = -g -Wall

PGM = ptar
TARGET = ./$(PGM)

$(shell mkdir -p $(BUILDDIR))
$(shell mkdir -p bin)


all: createArchive ptar

# Generic task
%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $(BUILDDIR)/$@

ptar: header_posix_ustar.o ptar.o
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)


### Create tar archive for tests
TAR = archive.tar
FILES = README.md LICENSE
createArchive:
	@echo "# Create archive for test"
	tar --format=posix -cf $(TAR) $(FILES)
	@echo