CFLAGS=-Wall -g

all: main

main: dyn.o

dyn.o: dyn.c

clean:
	rm main *.o
