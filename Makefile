INCDIR = include
SRCDIR = src
BUILDDIR = build
OBJ = $(wildcard $(BUILDDIR)/*.o)

INC = -I $(INCDIR)
CC = gcc
CFLAGS = -g -Wall $(INC)

PGM = ptar
TARGET = bin/$(PGM)

$(shell mkdir -p $(BUILDDIR))
$(shell mkdir -p bin)


all: ptar

ptar:
	@echo "Nothing to compile for the moment"
	
# Generic task
%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $(BUILDDIR)/$@