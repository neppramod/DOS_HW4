all: hw4

hw4: 
	g++ -o hw4 main.cpp content.cpp communication.cpp server.cpp client.cpp -lpthread

clean:
	rm hw4 *.o


