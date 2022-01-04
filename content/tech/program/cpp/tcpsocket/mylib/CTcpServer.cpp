////////////////////////////////////////////////////////////////////////////
//
//  TCP/IP stream socket server クラス
//
////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cstring>  // memset
using namespace std;
#include "CTcpServer.h"


//==========================================================================
// コンストラクタ (サーバのポート番号と listen 用キューサイズを指定)
//==========================================================================
CTcpServer::CTcpServer(short nPort, int queueSize /*= 5 */)
{
    // Create a TCP/IP stream socket.
    m_servSock = socket(AF_INET, SOCK_STREAM, 0);
    if (m_servSock == -1) {
        cerr << "Error: Can't create socket!" << endl;
        exit(1);
    }

    // bind２分制限を解除
    int val = 1;
    setsockopt(m_servSock, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    // Bind the name to the socket.	
    sockaddr_in servAddr;
    memset((void *) &servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(nPort);
    servAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(m_servSock, (sockaddr *) &servAddr, sizeof(servAddr)) == -1) {
        cerr << "Error: Can't bind the name to the socket! [port: "
            << nPort << "]" << endl;
        exit(1);
    }

    // Listen (Create a queue).
    if (listen(m_servSock, queueSize) == -1) {
        cerr << "Error: Can't listen" << endl;
        exit(1);
    }
}

