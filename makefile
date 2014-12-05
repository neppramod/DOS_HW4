all: hw4

hw4: communication.o client.o content.o main.o utility.o server.o
	g++ -o hw4 main.o content.o communication.o server.o client.o utility.o -lpthread
content.o:
	g++ -c content.cpp
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
	rm hw4 *.o
