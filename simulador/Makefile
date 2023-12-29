build: simulador

clean:
	rm -f *.o

simulador: simulador.o config.o socket.o utilizador.o util.o cJSON.o
	gcc -g simulador.o config.o socket.o utilizador.o util.o cJSON.o -lpthread -o simulador

simulador.o: simulador.c config.h socket.h utilizador.h
	gcc -g -c simulador.c

config.o: config.c config.h structs.h cJSON.h
	gcc -g -c config.c

socket.o: socket.c socket.h util.h
	gcc -g -c socket.c

utilizador.o: utilizador.c utilizador.h structs.h socket.h util.h
	gcc -g -c utilizador.c

util.o: util.c util.h
	gcc -g -c util.c

cJSON.o: cJSON.c cJSON.h
	gcc -g -c cJSON.c
