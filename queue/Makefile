.PHONY: atomic_queue.o main.o test

LDFLAGS="-pthread"
CFLAGS="-ggdb"

all: test main.o atomic_queue.o

atomic_queue.o: atomic_queue.c
	gcc -o $@ -c $< $(CFLAGS)

main.o: main.c
	gcc -o $@ -c $< $(CFLAGS) $(LDFLAGS)

test: main.o atomic_queue.o
	gcc -o $@ main.o atomic_queue.o $(LDFLAGS)

clean:
	rm -f *.o
