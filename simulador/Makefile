simulador: simulador.o config.o cJSON.o
	gcc -g simulador.o config.o cJSON.o -o simulador

simulador.o: simulador.c config.h
	gcc -g -c simulador.c

config.o: config.c config.h structs.h cJSON.h
	gcc -g -c config.c

cJSON.o: cJSON.c cJSON.h
	gcc -g -c cJSON.c

clean:
	rm -f *.o
