#include <iostream>
#include <cstring>
#include <winsock2.h>

#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#define DEFAULT_BUFLEN 1024

using namespace std;

int main(){
    //----------------------
    // Declare and initialize variables.
    int iResult;
    WSADATA wsaData;

    SOCKET ConnectSocket = INVALID_SOCKET;
    struct sockaddr_in clientService; 

    int recvbuflen = DEFAULT_BUFLEN;
    char recvbuf[DEFAULT_BUFLEN] = "";

    //----------------------
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    //----------------------
    // Create a SOCKET for connecting to server
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port of the server to be connected to.
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    clientService.sin_port = htons( 5150 );

    //----------------------
    // Connect to server.
    iResult = connect( ConnectSocket, (SOCKADDR*) &clientService, sizeof(clientService) );
    if (iResult == SOCKET_ERROR) {
        wprintf(L"connect failed with error: %d\n", WSAGetLastError() );
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    int type;
    char message[1024];
    char SendBuf[1024];
    char RecvBuf[1024];
    int BufLen = 1024;
    int sResult;
    int count = 0;
    
    cout << "---Menu---" << endl;
    cout << "1. Read all existing messages." << endl;
    cout << "2. Write a new message." << endl;
    cout << "Please type \"1\" or \"2\" to select an option:" << endl; 
    
    while (cin >> SendBuf[0]) {
        if (SendBuf[0] == '1') {
            if (count == 1){
                iResult = send( ConnectSocket, SendBuf, (int)strlen(SendBuf), 0 );
                cout << "All messages:" << endl;
                // for (int k = 0; k < count; k++)
                //     recv(ConnectSocket, RecvBuf[k], BufLen, 0);
                // for (int k = 0; k < count; k++)
                //     cout << RecvBuf[k] << endl;

                recv(ConnectSocket, RecvBuf, BufLen, 0);
                RecvBuf[strlen(RecvBuf)-1] = '\0';
                printf(RecvBuf);
                memset(RecvBuf, NULL, sizeof(RecvBuf));
            }
            

        } else if (SendBuf[0] == '2'){
            cout << "Type a new message:" << endl;
            cin >> message;
            strcat(SendBuf, message);
            //----------------------
            // Send an initial buffer
            iResult = send( ConnectSocket, SendBuf, (int)strlen(SendBuf), 0 );
            count = 1;
            if (iResult == SOCKET_ERROR) {
                wprintf(L"send failed with error: %d\n", WSAGetLastError());
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            }

            //printf("Bytes Sent: %d\n", iResult);
            memset(SendBuf, NULL, sizeof(SendBuf));
        }

        cout << "---Menu---" << endl;
        cout << "1. Read all existing messages." << endl;
        cout << "2. Write a new message." << endl;
        cout << "Please type \"1\" or \"2\" to select an option:" << endl;
    }

    closesocket(ConnectSocket);
    WSACleanup();
    return 0;
}