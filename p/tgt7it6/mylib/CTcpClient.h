////////////////////////////////////////////////////////////////////////////
//
//  TCP/IP stream socket client クラス
//
////////////////////////////////////////////////////////////////////////////
#ifndef __CTCPCLIENT_H__
#define __CTCPCLIENT_H__

#include <iostream>
#include "socket.h"


class CTcpClient {
public:
    CTcpClient();
    CTcpClient(int sockfd);
    virtual ~CTcpClient() { Close(); }
    // Connection
    bool TargetServer(const char *szHost, short nPort);
    bool Connect();
    void Close();
    // Transmission
    int Recv(void *buf, size_t len, int flags = 0);
    int Send(const void *msg, size_t len, int flags = 0); 
    bool SendAll(const void *msg, size_t len, int flags = 0);
    // Attributes
    int GetSockfd() const { return m_sockfd; }
    bool IsConnect() const { return m_bConnect; }
    // Helper
    static const hostent *GetHostent(const char *szHostnameOrHostaddr);
protected:
    int m_sockfd;            // ソケットファイルディスクリプタ
    bool m_bConnect;         // 接続中か？
    sockaddr_in m_servAddr;  // 接続するサーバのアドレス
};


//===========================================================================
// コンストラクタ1 (後で TargetServer(), Connect() を実行する必要がある)
//===========================================================================
inline CTcpClient::CTcpClient() : m_bConnect(false)
{
}

//===========================================================================
// コンストラクタ2 (すでに接続済みのソケット記述子を指定する)
//
// 主に accept() の戻り値をこの引数にセットして使うことになる。
// 当然このコンストラクタを使った場合は、ソケットは接続済みなので、
// TargetServer() や Connect() などを実行する必要はない。
//===========================================================================
inline CTcpClient::CTcpClient(int sockfd)
    : m_sockfd(sockfd), m_bConnect(true)
{
}

//===========================================================================
// recv() のカプセル化
//===========================================================================
inline int CTcpClient::Recv(void *buf, size_t len, int flags)
{
    return ::recv(m_sockfd, buf, len, flags);
}

//===========================================================================
// send() のカプセル化
//===========================================================================
inline int CTcpClient::Send(const void *msg, size_t len, int flags)
{
    return ::send(m_sockfd, msg, len, flags);
}


#endif // __CTCPCLIENT_H__

