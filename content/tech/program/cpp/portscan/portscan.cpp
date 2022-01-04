/////////////////////////////////////////////////////////////////////////////
//
//  Tcp port scanner.
//  Usage: portscan TargetHost [MaxPortNo]
//
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cstdlib>  // atoi
using namespace std;
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>


#define DEF_MAX_PORTS 1024
sockaddr_in g_addr;


//===========================================================================
// プログラム引数エラー
//===========================================================================
void Usage()
{
    cerr << "Usage: Portscan TargetHost [MaxPortNo]" << endl;
}

//===========================================================================
// hostentへのポインタを取得
//===========================================================================
const hostent *GetHostent(const char *szHostnameOrHostaddr)
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

//===========================================================================
// ターゲットホストのアドレスをセット
//===========================================================================
bool InitSockaddr(const char *szHost)
{
    const hostent *phe = GetHostent(szHost);
    if (phe == NULL)
        return false;

    g_addr.sin_family = AF_INET;
    memcpy((void *) &g_addr.sin_addr, (void *) phe->h_addr, phe->h_length);

    return true;
}

//===========================================================================
// ポートが開いているか？
//===========================================================================
bool IsPortOpen(uint16_t nPort)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        cerr << "Error: Create socket error!" << endl;
        exit(1);
    }

    g_addr.sin_port = htons(nPort);
    int nRet = connect(sockfd, (sockaddr *) &g_addr, sizeof(g_addr));
    close(sockfd);

    return nRet != -1;
}

//===========================================================================
// Main function
//===========================================================================
int main(int argc, char **argv)
{
    if (argc < 2) {
        Usage();
        exit(1);
    }

    if (InitSockaddr(argv[1]) == false) {
        cerr << "Error: Can't find the host." << endl;
        exit(1);
    }

    // どのポート番号までスキャンするか？
    int nMaxPort;
    if (argc >= 3)
        nMaxPort = (atoi(argv[2]) >=  65535) ? 65535 : atoi(argv[2]);
    else
        nMaxPort = DEF_MAX_PORTS;

    // 応答のあったポート番号表示
    cout << "Scanning TCP ports 1 through " << nMaxPort << endl;
    cout << "Running port is ... " << endl;
    for (int i = 1; i <= nMaxPort; i++) {
        if (IsPortOpen(i)) {
            servent *pse = getservbyport(htons(i), "tcp");
            if (pse != NULL)
                cout << i << "/tcp (" << pse->s_name << ")" << endl;
            else
                cout << i << "/tcp (Unknown)" << endl;
        }
    }

    return 0;
}

