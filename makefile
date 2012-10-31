all: main.o
	gcc output/main.o -o output/sws

main.o:
	gcc -c src/main.c -o output/main.o

clean:
	rm -rf output/*