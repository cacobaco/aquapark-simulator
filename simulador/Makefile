simulador: simulador.o config.o utilizador.o cJSON.o
	gcc -g simulador.o config.o utilizador.o cJSON.o -lpthread -o simulador

simulador.o: simulador.c config.h
	gcc -g -c simulador.c

config.o: config.c config.h structs.h cJSON.h
	gcc -g -c config.c

utilizador.o: utilizador.c utilizador.h structs.h
	gcc -g -c utilizador.c

cJSON.o: cJSON.c cJSON.h
	gcc -g -c cJSON.c

clean:
	rm -f *.o
