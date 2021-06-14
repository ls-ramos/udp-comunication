# My first makefile

all: client server

client: client.o serialize.o
	gcc -g -o client client.o serialize.o

server: server.o serialize.o
	gcc -g -o server server.o serialize.o

serialize: serialize.o
	gcc -g -o serialize serialize.o

client.o: client.c serialize.h
	gcc -g -o client.o client.c -c -W -Wall -ansi -pedantic -std=c99

server.o: server.c serialize.h
	gcc -g -o server.o server.c -c -W -Wall -ansi -pedantic -std=c99

serialize.o: serialize.c serialize.h unp.h
	gcc -g -o serialize.o serialize.c -c -W -Wall -ansi -pedantic -std=c99

clean:
	rm -rf *.o *~ client server