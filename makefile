all: main.o
	gcc main.o -o sws

main.o:
	gcc -c main.c 

clean:
	rm -rf *.o sws