CFLAGS=-Wall -g

all: main

main: dyn.o coupled.o

clean:
	rm main *.o
