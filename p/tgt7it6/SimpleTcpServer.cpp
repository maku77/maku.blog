#include <iostream>
using namespace std;
#include "mylib/CTcpServer.h"
#include "mylib/CTcpClient.h"

#define PORT 8324

int main()
{
    CTcpServer tcpServer(PORT);

    int clieSock = tcpServer.accept();
    if (clieSock == -1) {
        cerr << "Error: Accept error!" << endl;
        return 1;
    }

    CTcpClient tcpClient(clieSock);

    char *msg = "Message from server.";
    if (tcpClient.sendAll(msg, strlen(msg)+1) == false) {
        cerr << "Error: Send error!" << endl;
        return 1;
    }

    return 0;
}

