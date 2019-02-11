CC=gcc
CFLAGS=-c -Wall -g
LDFLAGS=-g -lm

WITH_MADVISE_MAIN=./src/with_madvise.c
WITHOUT_MADVISE_MAIN=./src/without_madvise.c

SOURCES=$(wildcard ./src/*.c)
OBJECTS=$(SOURCES:.c=.o)
WITH_MADVISE_OBJECTS=$(filter-out $(WITHOUT_MADVISE_MAIN:.c=.o), $(OBJECTS))
WITHOUT_MADVISE_OBJECTS=$(filter-out $(WITH_MADVISE_MAIN:.c=.o), $(OBJECTS))

WITH_MADVISE_EXECUTABLE=./bin/with_madvise
WITHOUT_MADVISE_EXECUTABLE=./bin/without_madvise

.PHONY: all bin clean

all: bin

bin: $(WITH_MADVISE_EXECUTABLE) $(WITHOUT_MADVISE_EXECUTABLE)

$(WITH_MADVISE_EXECUTABLE): $(WITH_MADVISE_OBJECTS)
	$(CC) $(LDFLAGS) $(WITH_MADVISE_OBJECTS) -o $@

$(WITHOUT_MADVISE_EXECUTABLE): $(WITHOUT_MADVISE_OBJECTS)
	$(CC) $(LDFLAGS) $(WITHOUT_MADVISE_OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm $(OBJECTS)
	-rm $(WITH_MADVISE_EXECUTABLE)
	-rm $(WITHOUT_MADVISE_EXECUTABLE)

run-with-madvise:
	./bin/with_madvise & ./bin/with_madvise & ./bin/with_madvise & ./bin/with_madvise & ./bin/with_madvise & ./bin/with_madvise

run-without-madvise:
	./bin/without_madvise & ./bin/without_madvise & ./bin/without_madvise & ./bin/without_madvise & ./bin/without_madvise & ./bin/without_madvise

run:
	make run-with-madvise & make run-without-madvise