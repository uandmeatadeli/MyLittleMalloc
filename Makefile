CC = gcc
CFLAGS = -Wall -fsanitize=address -std=c99 -O2 -g

test: test.o 
	$(CC) $(CFLAGS) test.o -o test

memgrind: memgrind.o mymalloc.o
	$(CC) $(CFLAGS) memgrind.o mymalloc.o -o memgrind

memtest: memtest.o mymalloc.o
	$(CC) $(CFLAGS) memtest.o mymalloc.o -o memtest


mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -c -Wall mymalloc.c

memtest.o: memtest.c mymalloc.h
	$(CC) $(CFLAGS) -c -Wall memtest.c

memgrind.o: memgrind.c mymalloc.h
	$(CC) $(CFLAGS) -c -Wall memgrind.c

test.o: test.c mymalloc.c
	$(CC) $(CFLAGS) -c -Wall test.c

clean:
	rm *.o test
