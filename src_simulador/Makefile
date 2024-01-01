build: simulador

clean:
	rm -f *.o

simulador: simulador.o config.o utilizador.o socket_client.o util.o cJSON.o
	gcc -g simulador.o config.o utilizador.o socket_client.o util.o cJSON.o -lpthread -o simulador

simulador.o: simulador.c config.h socket_client.h utilizador.h util.h
	gcc -g -c simulador.c

config.o: config.c config.h structs.h cJSON.h
	gcc -g -c config.c

utilizador.o: utilizador.c utilizador.h structs.h config.h socket_client.h util.h
	gcc -g -c utilizador.c

socket_client.o: socket_client.c socket_client.h util.h
	gcc -g -c socket_client.c

util.o: util.c util.h
	gcc -g -c util.c

cJSON.o: cJSON.c cJSON.h
	gcc -g -c cJSON.c
