CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

P2POBJS =		P2P.o BasicClient.o BasicServer.o FileReceiver.o FileSender.o 
RSOBJS = 		RS/main.o

P2PLIBS = 		
RSLIBS =		

VPATH = ../

TARGET =	P2P RS

all:	P2P RS

P2P:	$(P2POBJS)
	$(CXX) -o Client $(P2POBJS) $(P2PLIBS)

RS:	$(RSOBJS)
	$(CXX) -o RegServ $(RSOBJS) $(RSLIBS)

clean:
	rm -rf *.o
