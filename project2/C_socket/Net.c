#include <stdio.h>
#include <winsock2.h>
#include "test.h"
#define default_port 5000
#pragma comment (lib, "ws2_32.lib")

char* getIP(); 
void Start_Server(void);
void Start_Server(void)
{
    char *ip;
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //ip = getIP();
    ip = "127.0.0.1";
    puts(ip);

    SOCKET ServeSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in Serve_addr;
    memset(&Serve_addr, 0, sizeof(Serve_addr));
    Serve_addr.sin_family = AF_INET;
    Serve_addr.sin_addr.s_addr = inet_addr(ip);
    Serve_addr.sin_port = htons(default_port);
    
    bind(ServeSocket, (SOCKADDR*)&Serve_addr, sizeof(SOCKADDR));
    listen(ServeSocket, 1);
    puts("start listening...");

    struct sockaddr_in ClientAddr;
    int nSize = sizeof(SOCKADDR);
    SOCKET ClientSocket = accept(ServeSocket, (SOCKADDR*)&ClientAddr, &nSize);
    puts("get connected.");
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
                    puts("server send");
                    break;
                }
        }
        char recvData[1024] ={0};
        int ret = recv(ClientSocket, recvData, 1024,0);
        if (ret > 0)
        {
           strcat(recvData, "Server.");
           Get_Data = recvData;
        } 
    }
}   
char* getIP()  
{  
    
    //WSADATA wsaData;
    //WSAStartup( MAKEWORD(2, 2), &wsaData); // 加载套接字库    
  
    int i = 0;  
    struct hostent *phostinfo = gethostbyname("");    
    for(i = 0; NULL != phostinfo&& NULL != phostinfo->h_addr_list[i]; ++i)  
    {  
        char *pszAddr = inet_ntoa(*(struct in_addr *)phostinfo->h_addr_list[i]);  
        return pszAddr;  
    }  
  
    //WSACleanup( );    
} 

void Start_Client(char * ip);
void Start_Client(char * ip)
{
    //char *ip;
    //ip = "127.0.0.1";
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(ip);
    sock_addr.sin_port = htons(default_port);
    
    connect(sock, (SOCKADDR *)&sock_addr, sizeof(SOCKADDR));
    puts("connect successfully.");
    while (1)
    {
        char recvData[1024]={0};
        int ret = recv(sock, recvData, 1024, 0);
        if (ret > 0)
        {
            strcat(recvData, "Client.");
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
                    puts("client send");
                    break;
                }
        }
    }
}