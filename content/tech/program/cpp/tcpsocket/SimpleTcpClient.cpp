#include <iostream>
using namespace std;
#include "mylib/CTcpClient.h"

#define PORT 8324

int main()
{
    CTcpClient tcpClient;

    if (tcpClient.targetServer("127.0.0.1", PORT) == false) {
        cerr << "Error: Where is server?" << endl;
        return 1;
    }

    if (tcpClient.connect() == false) {
        cerr << "Error: Can't connect to the server!" << endl;
        return 1;
    }

    // Receive message
    char buf[100];
    int nRecv;
    while ((nRecv = tcpClient.recv(buf, sizeof(buf))) > 0) {
        cout << buf << endl;
    }

    return 0;
}

