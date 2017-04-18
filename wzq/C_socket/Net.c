#include <stdio.h>
#include <winsock2.h>
#include "NetThread.h"
#define default_port 5000
#pragma comment (lib, "ws2_32.lib")
char* getIP(); 
void Start_Server(void);
void Start_Server(void)
{
    char *ip;
    int ret;
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    ip = getIP();

    SOCKET ServeSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in Serve_addr;
    memset(&Serve_addr, 0, sizeof(Serve_addr));
    Serve_addr.sin_family = AF_INET;
    Serve_addr.sin_addr.s_addr = inet_addr(ip);
    Serve_addr.sin_port = htons(default_port);

    ret = bind(ServeSocket, (SOCKADDR*)&Serve_addr, sizeof(SOCKADDR));
    if (ret == SOCKET_ERROR)
    {
        printf("Bind Error!");
        closesocket(ServeSocket);
        WSACleanup();
        return;
    }

    ret = listen(ServeSocket, 1);
    if (ret == SOCKET_ERROR)
    {
        printf("Listen Error!");
        closesocket(ServeSocket);
        WSACleanup();
        return;
    }
    printf("start listening...");

    struct sockaddr_in ClientAddr;
    int nSize = sizeof(SOCKADDR);
    SOCKET ClientSocket = accept(ServeSocket, (SOCKADDR*)&ClientAddr, &nSize);
    if(ClientSocket == INVALID_SOCKET)
    {
        printf("Accept socket Error!");
        closesocket(ServeSocket);
        WSACleanup();
        return;
    }
    printf("Accept one connect.")
    IsConnected = 1;
    while (1)
    {
        while(1)
        {  
            if (Queue != NULL)
                if(Queue[0] == 'Q')
                {
                    closesocket(ClientSocket);
                    closesocket(ServeSocket);
                    WSACleanup();
                    return;
                }
                else
                {
                    send(ClientSocket, Queue, strlen(Queue), 0);
                    Queue = NULL;
                    break;
                }
        }
        char recvData[1024] ={0};
        int ret = recv(ClientSocket, recvData, 1024,0);
        if (ret > 0)
        {
           Get_Data = recvData;
        } 
    }
}   
char* getIP()  
{    
    int i = 0;  
    struct hostent *phostinfo = gethostbyname("");    
    for(i = 0; NULL != phostinfo&& NULL != phostinfo->h_addr_list[i]; ++i)  
    {  
        char *pszAddr = inet_ntoa(*(struct in_addr *)phostinfo->h_addr_list[i]);  
        return pszAddr;  
    }  
} 

void Start_Client(char * ip);
void Start_Client(char * ip)
{
    //char *ip;
    //ip = "127.0.0.1";
    int ret;
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(ip);
    sock_addr.sin_port = htons(default_port);
    
    ret = connect(sock, (SOCKADDR *)&sock_addr, sizeof(SOCKADDR));
    if (ret == SOCKET_ERROR)
    {
        printf("Connect Error!Please Check IP");
        closesocket(sock);
        WSACleanup();
        return;
    }
    printf("Successfully connected!");
    IsConnected = 1;
    while (1)
    {
        char recvData[1024]={0};
        int ret = recv(sock, recvData, 1024, 0);
        if (ret > 0)
        {
            Get_Data = recvData;
        }
        while (1)
        { 
            if (Queue != NULL)
                if (Queue[0] == 'Q')
                {
                    closesocket(sock);
                    WSACleanup();
                    return;
                }
                else
                {
                    send(sock, Queue, strlen(Queue), 0);
                    Queue = NULL;
                    break;
                }
        }
    }
}