files: server client

all: server.o client.o
	g++ server.cpp stack.cpp -o server -lpthread
	g++ client.cpp -o client

.PHONY: clean all

clean:
	rm -f *.o server client stack
