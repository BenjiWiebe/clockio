EXE=clockio.exe
CFLAGS=-Wextra -Wall -std=c99
LDFLAGS=-llctp -mwindows -lcomctl32

SOURCES=$(wildcard *.c)
RESOURCES=$(wildcard *.rc)
OBJECTS=$(SOURCES:.c=.o)
RESOBJECTS=$(RESOURCES:.rc=.res)

ARCH=x86_64-w64-mingw32-
CC=$(ARCH)gcc
WINDRES=$(ARCH)windres

all: $(EXE)

$(EXE): $(OBJECTS) $(RESOBJECTS)
	$(CC) $(OBJECTS) $(RESOBJECTS) $(LDFLAGS) -o $(EXE)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.res: %.rc
	$(WINDRES) -J rc -O coff -i $< -o $@

clean:
	-rm -f $(EXE) $(OBJECTS) $(RESOBJECTS)

