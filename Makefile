CC=gcc
CFLAGS=-Wall

dcooke_parser: P2.o front.h parser.h
	$(CC) P2.o -o dcooke_parser
P2.o: P2.c front.h parser.h
	$(CC) -c P2.c -o P2.o $(CFLAGS)
clean:
	rm -f P2.o dcooke_parser