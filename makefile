all: sws

sws: main.o
	gcc main.o -o sws

main.o: main.c
	gcc -c main.c

clean:
	rm -rf *o sws