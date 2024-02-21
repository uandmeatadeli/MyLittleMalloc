CC = gcc
CFLAGS = -Wall -fsanitize=address -std=c99 -O2 -g

all: memgrind memtest Hardtesting

memgrind: memgrind.o mymalloc.o
	$(CC) $(CFLAGS) memgrind.o mymalloc.o -o memgrind

memtest: memtest.o mymalloc.o
	$(CC) $(CFLAGS) memtest.o mymalloc.o -o memtest

Hardtesting: Hardtesting.o mymalloc.o
	$(CC) $(CFLAGS) Hardtesting.o mymalloc.o -o Hardtesting

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -c -Wall mymalloc.c

memtest.o: memtest.c mymalloc.h
	$(CC) $(CFLAGS) -c -Wall memtest.c

memgrind.o: memgrind.c mymalloc.h
	$(CC) $(CFLAGS) -c -Wall memgrind.c

Hardtesting.o: Hardtesting.c mymalloc.h
	$(CC) $(CFLAGS) -c -Wall Hardtesting.c

clean:
	rm -f *.o memgrind memtest Hardtesting
