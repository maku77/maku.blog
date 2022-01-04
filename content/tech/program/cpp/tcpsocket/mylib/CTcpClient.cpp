////////////////////////////////////////////////////////////////////////////
//
//  TCP/IP stream socket client クラス
//  TargetServer() でサーバを指定し、Connect() 実行後に通信可能となる。
//
////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cstring>  // memset
using namespace std;
#include "CTcpClient.h"


//===========================================================================
// 接続するサーバの名前、ポート番号を入力する。(Connect()の前に実行)
// サーバが見付からなかった場合は false を返す。
//
// parameters:
//  szHost -- サーバのホスト名または、IPアドレスのドット区切り10進記法
//  nPort  -- ポート番号
//===========================================================================
bool CTcpClient::TargetServer(const char *szHost, short nPort)
{
    // Get server's info
    const hostent *pServHost = CTcpClient::GetHostent(szHost);
    if (pServHost == NULL) {
        return false;
    }

    // Fill in the address structure for the server
    // and Connect to the server.
    m_servAddr.sin_family = AF_INET;
    m_servAddr.sin_port = htons(nPort);
    memcpy((void *) &m_servAddr.sin_addr, (void *) pServHost->h_addr,
            pServHost->h_length);

    return true;
}

//===========================================================================
// サーバへ接続する。(この前に TargetServer() を実行する必要がある)
//===========================================================================
bool CTcpClient::Connect()
{
    // Create a TCP/IP stream socket.
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockfd == -1)
        return false;

    // Connect to the server.	
    if (::connect(m_sockfd, (sockaddr *) &m_servAddr, sizeof(m_servAddr)) == -1)
        return false;

    m_bConnect = true;  // つながったよー。
    return true;
}

//===========================================================================
// サーバとのコネクションを切る。
// (TargetServer() 実行していれば、再び Connect() することも可能)
//===========================================================================
void CTcpClient::Close()
{
    ::shutdown(m_sockfd, 2);  // SD_BOTH
    ::close(m_sockfd);

    m_bConnect = false;
}

//===========================================================================
// len バイトのデータをすべて Send() する。
//===========================================================================
bool CTcpClient::SendAll(const void *msg, size_t len, int flags)
{
    const char *pData = (const char *) msg;  // バイト単位で操作したい

    do {
        int nSent = Send(pData, len, flags);
        if (nSent == -1)
            return false;  // 失敗だー

        pData += nSent;  // 送ったバイト数だけポインタを進める
        len -= nSent;    // あと何バイト送ればいいか？
    } while (len > 0);

    return true;  // 無事送りきった
}

//===========================================================================
// ホスト名または、10進記法IPアドレスから hostent へのポインタを獲得する。
// 失敗したときは NULL を返す。
// Example:
//   hostent *pHostent = GetHostent("127.0.0.1");
//   hostent *pHostent = GetHostent("www.puni.mohe");
//===========================================================================
const hostent *CTcpClient::GetHostent(const char *szHostnameOrHostaddr)
{
    in_addr ia;  // インターネットアドレス

    // inet_aton() でホスト名かIPアドレスか調べる
    if (inet_aton(szHostnameOrHostaddr, &ia) == 0) {
        // IPアドレスじゃないよー。たぶんホスト名だね。
        return gethostbyname(szHostnameOrHostaddr);
    } else {
        // IPアドレスだね。
        return gethostbyaddr((const char *) &ia, sizeof(in_addr), AF_INET);
    }
}

