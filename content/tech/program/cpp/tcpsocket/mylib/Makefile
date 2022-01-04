CC = g++
CFLAGS = -O2

all: CTcpClient.o CTcpServer.o

CTcpClient.o: CTcpClient.cpp CTcpClient.h socket.h
	$(CC) -c $(CFLAGS) CTcpClient.cpp

CTcpServer.o: CTcpServer.cpp CTcpServer.h socket.h
	$(CC) -c $(CFLAGS) CTcpServer.cpp

#
# Cleanup .o files
#
clean:
	rm *.o

