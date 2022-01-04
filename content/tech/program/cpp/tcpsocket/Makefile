CC = g++
LIB_CLIENT = ../../mylib/TcpSock/CTcpClient.o
LIB_SERVER = ../../mylib/TcpSock/CTcpServer.o

all:	SimpleTcpClient SimpleTcpServer

SimpleTcpClient: SimpleTcpClient.cpp
	$(CC) -o $@ SimpleTcpClient.cpp $(LIB_CLIENT)

SimpleTcpServer: SimpleTcpServer.cpp
	$(CC) -o $@ SimpleTcpServer.cpp $(LIB_CLIENT) $(LIB_SERVER)
