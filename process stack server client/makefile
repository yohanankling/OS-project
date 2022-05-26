files: server client

all: server.o client.o
	g++ server.cpp stack.cpp -o server
	g++ client.cpp -o client
	g++ test.cpp -o test1
	g++ test2.cpp -o test2

.PHONY: clean all

clean:
	rm -f *.o server client stack test1 test2
