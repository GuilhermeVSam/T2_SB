.PHONY: all clean

all: main

main: main.o mymemory.o
	gcc -o main main.o mymemory.o

main.o: main.c
	gcc -c main.c

mymemory.o: mymemory.c
	gcc -c mymemory.c

clean:
	rm -f main *.o