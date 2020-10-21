CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system
CFLAGS += -pthread
LDFLAGS += -pthread

all: StompBookClubClient
	g++ -o bin/StompBookClubClient bin/bookDetails.o bin/clientInfo.o bin/readFromSocket.o bin/STOMPProtocol.o bin/connectionHandler.o bin/echoClient.o $(LDFLAGS) 

StompBookClubClient: bin/bookDetails.o bin/clientInfo.o bin/readFromSocket.o bin/STOMPProtocol.o bin/connectionHandler.o bin/echoClient.o

bin/bookDetails.o: src/bookDetails.cpp include/bookDetails.h
	g++ $(CFLAGS) -o bin/bookDetails.o src/bookDetails.cpp
	
bin/clientInfo.o:  src/clientInfo.cpp include/clientInfo.h
	g++ $(CFLAGS) -o bin/clientInfo.o src/clientInfo.cpp

bin/connectionHandler.o: src/connectionHandler.cpp include/connectionHandler.h
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/readFromSocket.o: src/readFromSocket.cpp include/readFromSocket.h
	g++ $(CFLAGS) -o bin/readFromSocket.o src/readFromSocket.cpp

bin/STOMPProtocol.o : src/STOMPProtocol.cpp include/STOMPProtocol.h
	g++ $(CFLAGS) -o bin/STOMPProtocol.o src/STOMPProtocol.cpp

bin/echoClient.o : src/echoClient.cpp
	g++ $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp

.PHONY: clean
clean:
	rm -f bin/*
