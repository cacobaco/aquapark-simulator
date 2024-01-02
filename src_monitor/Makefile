build: monitor

clean:
	rm -f *.o

monitor: monitor.o socket_server.o util.o logs.o
	gcc -g monitor.o socket_server.o util.o logs.o -o monitor

monitor.o: monitor.c socket_server.h logs.h
	gcc -g -c monitor.c

socket_server.o: socket_server.c socket_server.h util.h logs.h
	gcc -g -c socket_server.c

util.o: util.c util.h
	gcc -g -c util.c

logs.o: logs.c logs.h
	gcc -g -c logs.c
