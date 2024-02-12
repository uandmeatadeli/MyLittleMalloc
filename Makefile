CC = gcc
CFLAGS = -Wall -fsanitize=address -std=c99 -02

memgrind: memgrind.o mymalloc.o
	$(CC) $(CFLAGS) memgrind.o mymalloc.o -o memgrind

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -c -WALL mymalloc.c

memgrind.o: memgrind.c mymalloc.h
	$(CC) $(CFLAGS) -c -Wall memgrind.c

clean:
	rm *.o test
