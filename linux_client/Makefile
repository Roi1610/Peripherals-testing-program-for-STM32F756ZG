all: client

client: client_interface.o 
	gcc -ggdb -Wall -pedantic client_interface.o -o client

tftp_client.o: client_interface.c client_interface.h protocol.h
	gcc -ggdb -c client_interface.c


.PHONY: clean

clean:
	rm -f *.o client
