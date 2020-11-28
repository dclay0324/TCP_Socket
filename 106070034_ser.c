#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

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
    struct sockaddr_in serAddr;
    struct sockaddr_in RecvAddr;
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
    char SendBuf[1024];
    char Buffer[1024];
    int BufLen = 1024;
    int flag = 0;
    char Menu[] = "---Menu---\n1. Read all existing messages.\n2. Write a new message.\nPlease type \"1\" or \"2\" to select an option:\n";
    char AllMsg[] = "All messages:\n";
    char NewMsg[] = "Type a new message:\n";

    // Receive until the peer closes the connection
    do {
        send(AcceptSocket, Menu, (int)strlen(Menu), 0);
        iResult = recv(AcceptSocket, RecvBuf, BufLen, 0);
        if ( iResult > 0 ) {
            if (RecvBuf[0] == '1') {
                if (flag != 0) {
                    strcat(SendBuf, AllMsg);
                    strcat(SendBuf, Buffer);
                    strcat(SendBuf, "\n");

                    send(AcceptSocket, SendBuf, (int)strlen(SendBuf), 0);

                    memset(SendBuf, NULL, sizeof(SendBuf));
                    memset(RecvBuf, NULL, sizeof(RecvBuf));
                }
            } else if (RecvBuf[0] == '2') {
                memset(RecvBuf, NULL, sizeof(RecvBuf));

                send(AcceptSocket, NewMsg, (int)strlen(NewMsg), 0);
                recv(AcceptSocket, RecvBuf, BufLen, 0);

                strcat(Buffer, RecvBuf);
                strcat(Buffer, "\n");
 
                flag = 1;
                memset(RecvBuf, NULL, sizeof(RecvBuf));
            }
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
