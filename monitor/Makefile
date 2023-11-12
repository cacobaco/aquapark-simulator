monitor: monitor.o socket.o util.o
	gcc -g monitor.o socket.o util.o -o monitor

monitor.o: monitor.c socket.h
	gcc -g -c monitor.c

socket.o: socket.c socket.h util.h
	gcc -g -c socket.c

util.o: util.c util.h
	gcc -g -c util.c

clean:
	rm -f *.o
