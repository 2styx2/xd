CC=gcc
CFLAGS=-Wall
DEPS = src/xtrad.h
OBJ = src/xtrad.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
xtrad: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f xtrad src/xtrad.o
