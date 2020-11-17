#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#ifndef UNICODE
#define UNICODE
#endif

int main(int argc, char *argv[])
{
    int port = atoi(argv[1]);
//----------------------
    // Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"WSAStartup failed with error: %ld\n", iResult);
        return 1;
    }

    //----------------------
    SOCKET ListenSocket;
    sockaddr_in serAddr;
    sockaddr_in RecvAddr;
    socklen_t addr_len;

    //----------------------
    // Create a SOCKET for listening for
    // incoming connection requests.
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serAddr.sin_port = htons(port);

    if (bind(ListenSocket,
             (SOCKADDR *) & serAddr, sizeof (serAddr)) == SOCKET_ERROR) {
        wprintf(L"bind failed with error: %ld\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    //----------------------
    // Listen for incoming connection requests.
    // on the created socket
    if (listen(ListenSocket, 1) == SOCKET_ERROR) {
        wprintf(L"listen failed with error: %ld\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    //----------------------
    // Create a SOCKET for accepting incoming requests.
    SOCKET AcceptSocket;
    wprintf(L"Waiting for client to connect...\n");

    //----------------------
    // Accept the connection.
    //AcceptSocket = accept(ListenSocket, NULL, NULL);
    AcceptSocket = accept(ListenSocket, (SOCKADDR *) & RecvAddr, (socklen_t *) & addr_len);
    if (AcceptSocket == INVALID_SOCKET) {
        wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    } else
        wprintf(L"Client connected.\n");
    
    char RecvBuf[1024];
    char Buffer[1024];
    int BufLen = 1024;
    int count = 0;
    memset(RecvBuf, NULL, sizeof(RecvBuf));
   // Receive until the peer closes the connection
    do {
        iResult = recv(AcceptSocket, RecvBuf, BufLen, 0);
        if ( iResult > 0 ) {
            if (RecvBuf[0] == '1') {
                if (count != 0) {
                    printf(Buffer);
                    send( AcceptSocket, Buffer, (int)strlen(Buffer), 0 );
                }   
            } else if (RecvBuf[0] == '2') {
                //printf("Bytes received: %d\n", iResult);
                //printf("RecvBuf received: %s\n", RecvBuf);

                for (int j = 0; j < (int)strlen(RecvBuf)-1; j++) {
                    Buffer[count] = RecvBuf[j+1];
                    count++;
                }
                printf(Buffer);
                Buffer[count] = '\n';
                count++;
                //printf("%d", count);
            }
            memset(RecvBuf, NULL, sizeof(RecvBuf));
        }
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());

    } while( iResult > 0 );

    // No longer need server socket
    closesocket(ListenSocket);
    closesocket(AcceptSocket);
    WSACleanup();
    return 0;
}
