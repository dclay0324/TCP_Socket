//#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#ifndef UNICODE
#define UNICODE
#endif

//using namespace std;

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
    char type;
    char message[1024];
    char SendBuf[1024];
    char RecvBuf[1024];
    int BufLen = 1024;
    int flag = 0;

    memset(SendBuf, NULL, sizeof(SendBuf));
    memset(RecvBuf, NULL, sizeof(RecvBuf));
    
    // cout << "---Menu---" << endl;
    // cout << "1. Read all existing messages." << endl;
    // cout << "2. Write a new message." << endl;
    // cout << "Please type \"1\" or \"2\" to select an option:" << endl; 
    
    // while (cin >> SendBuf[0]) {
    //     if (SendBuf[0] == '1') {
    //         if (flag == 1){
    //             iResult = send( ConnectSocket, SendBuf, (int)strlen(SendBuf), 0 );
    //             cout << "All messages:" << endl;

    //             recv(ConnectSocket, RecvBuf, BufLen, 0);
    //             RecvBuf[(int)strlen(RecvBuf)-1] = '\0';
    //             printf(RecvBuf);
    //             memset(RecvBuf, NULL, sizeof(RecvBuf));
    //         }
    //     } else if (SendBuf[0] == '2'){
    //         cout << "Type a new message:" << endl;
    //         cin >> message;
    //         strcat(SendBuf, message);
    //         //----------------------
    //         // Send an initial buffer
    //         iResult = send( ConnectSocket, SendBuf, (int)strlen(SendBuf), 0 );
    //         flag = 1;
    //         if (iResult == SOCKET_ERROR) {
    //             wprintf(L"send failed with error: %d\n", WSAGetLastError());
    //             closesocket(ConnectSocket);
    //             WSACleanup();
    //             return 1;
    //         }
    //         //printf("Bytes Sent: %d\n", iResult);
    //         memset(SendBuf, NULL, sizeof(SendBuf));
    //     }

    //     cout << "\n---Menu---" << endl;
    //     cout << "1. Read all existing messages." << endl;
    //     cout << "2. Write a new message." << endl;
    //     cout << "Please type \"1\" or \"2\" to select an option:" << endl;
    // }

    while (1) {
        printf("---Menu---\n1. Read all existing messages.\n2. Write a new message.\nPlease type \"1\" or \"2\" to select an option:\n");
        char c = getchar();
        if (c == EOF) exit(1);
        else if (c == '1'){
            getchar();
        	SendBuf[0] = c;
		} else SendBuf[0] = c;

        if (SendBuf[0] == '1') {
            if (flag == 1){
                iResult = send( ConnectSocket, SendBuf, (int)strlen(SendBuf), 0 );
                if (iResult == SOCKET_ERROR) {
                    wprintf(L"send failed with error: %d\n", WSAGetLastError());
                    closesocket(ConnectSocket);
                    WSACleanup();
                    return 1;
                }
                printf("All messages:\n");

                recv(ConnectSocket, RecvBuf, BufLen, 0);
                for (int j = 0; j < (int)strlen(RecvBuf); j++){
                    if (RecvBuf[j] == '\0') printf("\n");
                    else printf("%c", RecvBuf[j]);
                }
                printf("\n");
                memset(SendBuf, NULL, sizeof(SendBuf));
                memset(RecvBuf, NULL, sizeof(RecvBuf)); 
            } else printf("There is no any messages.\n\n");
        } else if (SendBuf[0] == '2'){
            printf("Type a new message:\n");
            char msg = getchar();
            for (i = 1, msg = getchar(); msg != '\n'; msg = getchar(), i++) SendBuf[i] = msg;
            SendBuf[i] = '\0';
            
            //----------------------
            // Send an initial buffer
            iResult = send( ConnectSocket, SendBuf, (int)strlen(SendBuf), 0 );
            flag = 1;
            if (iResult == SOCKET_ERROR) {
                wprintf(L"send failed with error: %d\n", WSAGetLastError());
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            }
            //printf("Message Sent: %s\n", SendBuf);
            printf("\n");
            memset(SendBuf, NULL, sizeof(SendBuf));
        }
    }

    closesocket(ConnectSocket);
    WSACleanup();
    return 0;
}