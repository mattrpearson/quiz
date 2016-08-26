all: longest_compound.c
	gcc -Wall -o longest_compound longest_compound.c

clean:
	rm -rf longest_compound
	rm -rf *.o
