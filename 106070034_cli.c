#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#ifndef UNICODE
#define UNICODE
#endif

int main(int argc, char *argv[]){

    //----------------------
    // Declare and initialize variables.
    int iResult;
    WSADATA wsaData;

    SOCKET ConnectSocket = INVALID_SOCKET;
    struct sockaddr_in clientAddress; 

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
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_addr.s_addr = inet_addr( argv[1] );
    clientAddress.sin_port = htons( atoi(argv[2]) );

    //----------------------
    // Connect to server.
    iResult = connect( ConnectSocket, (SOCKADDR*) &clientAddress, sizeof(clientAddress) );
    if (iResult == SOCKET_ERROR) {
        wprintf(L"connect failed with error: %d\n", WSAGetLastError() );
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    int i;
    char SendBuf[1024];
    char RecvBuf[1024];
    char Menu[1024];
    int BufLen = 1024;
    int flag = 0;

    while (1) {
        recv(ConnectSocket, Menu, BufLen, 0);
        printf("%s", Menu);

        char c = getchar();
        if (c == EOF) exit(1);
        else if (c == '1'){
            getchar();
        	SendBuf[0] = c;
		} else SendBuf[0] = c;

        send(ConnectSocket, SendBuf, (int)strlen(SendBuf), 0);

        if (SendBuf[0] == '1') {
            if (flag == 1){
                recv(ConnectSocket, RecvBuf, BufLen, 0);
                printf("%s", RecvBuf);
                
                memset(SendBuf, NULL, sizeof(SendBuf));
                memset(RecvBuf, NULL, sizeof(RecvBuf)); 
                memset(Menu, NULL, sizeof(Menu));
            } else {
                printf("There is no any messages.\n\n");
            }
        } else if (SendBuf[0] == '2'){
            memset(SendBuf, NULL, sizeof(SendBuf));

            recv(ConnectSocket, RecvBuf, BufLen, 0);
            printf("%s", RecvBuf);
            
            char msg = getchar();
            for (i = 0, msg = getchar(); msg != '\n'; msg = getchar(), i++) SendBuf[i] = msg;
            printf("\n");
            //----------------------
            // Send an initial buffer
            iResult = send( ConnectSocket, SendBuf, (int)strlen(SendBuf), 0 );
            flag = 1;
            
            memset(RecvBuf, NULL, sizeof(RecvBuf));
            memset(SendBuf, NULL, sizeof(SendBuf));
            memset(Menu, NULL, sizeof(Menu));
        }
    }

    closesocket(ConnectSocket);
    WSACleanup();
    return 0;
}