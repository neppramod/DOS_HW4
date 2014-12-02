all: hw3

hw3: communication.o client.o main.o utility.o
	g++ -o hw3 communication.o client.o main.o utility.o server.o -lpthread
communication.o:
	g++ -c communication.cpp
server.o:
	g++ -c server.cpp
client.o: communication.o
	g++ -c client.cpp
utility.o:
	g++ -c utility.cpp
main.o: communication.o client.o utility.o server.o
	g++ -c main.cpp

clean:
	rm hw3 *.o


