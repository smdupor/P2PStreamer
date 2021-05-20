CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		P2P.o BasicClient.o BasicServer.o FileReceiver.o FileSender.o

LIBS =

TARGET =	P2P

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
