all: hw4

hw4: 
	g++ -o hw4 main.cpp content.cpp communication.cpp server.cpp client.cpp  utility.cpp dfs.cpp -lpthread

clean:
	rm hw4 *.o


