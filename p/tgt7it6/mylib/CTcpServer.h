////////////////////////////////////////////////////////////////////////////
//
//  TCP/IP stream socket server クラス
//
////////////////////////////////////////////////////////////////////////////
#ifndef __CTCPSERVER_H__
#define __CTCPSERVER_H__

#include <iostream>
#include "socket.h"


class CTcpServer {
public:
    CTcpServer(short nPort, int queueSize = 5);
    virtual ~CTcpServer() { Close(); }

    int Accept();
    void Close() const { ::close(m_servSock); }
    int GetSockfd() const { return m_servSock; }
private:
    CTcpServer();  // デフォルトのコンストラクト禁止
protected:
    int m_servSock;  // listen用ソケット記述子
};


//===========================================================================
// クライアントの接続を待機する。
// クライアントとの通信用ソケット記述子を返す。(エラーの場合は -1)
//===========================================================================
inline int CTcpServer::Accept()
{
    return ::accept(m_servSock, NULL, NULL);
}


#endif // __CTCPSERVER_H__
