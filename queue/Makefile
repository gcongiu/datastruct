.PHONY: atomic_queue.o main.o test

all: test main.o atomic_queue.o

atomic_queue.o: atomic_queue.c
	gcc -o $@ -c $< $(CFLAGS)

main.o: main.c
	gcc -o $@ -c $< $(CFLAGS)

test: main.o atomic_queue.o
	gcc -o $@ main.o atomic_queue.o $(CFLAGS) -pthread

clean:
	rm -f *.o
