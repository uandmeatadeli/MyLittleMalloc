CC = gcc
CFLAGS = -Wall -fsanitize=address -std=c99 -O2

memtest: memtest.o mymalloc.o
	$(CC) $(CFLAGS) memtest.o mymalloc.o -o memtest

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -c -Wall mymalloc.c

memtest.o: memtest.c mymalloc.h
	$(CC) $(CFLAGS) -c -Wall memtest.c

clean:
	rm *.o test
