#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include "include/graphics.h"
#include "NetThread.h"
#include "wzq.h"
#define default_port 5000
#pragma comment (lib, "ws2_32.lib")
char* getIP(); 
void Start_Server(void);

SOCKET init_server_sock();
SOCKET init_server_sock()
{
 	char *ip;
    int ret;
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    int location[2];

    ip = getIP();
    puts(ip);

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
    SOCKET Cli_sock = accept(ServeSocket, (SOCKADDR*)&ClientAddr, &nSize);
    if(Cli_sock == INVALID_SOCKET)
    {
        printf("Accept socket Error!");
        closesocket(ServeSocket);
        WSACleanup();
        return;
    }
    printf("Accept one connect.");
    IsConnected = 1;
    return Cli_sock;
}
void Start_Server(void)
{
    char *ip;
    int ret;
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    int location[2];

    ip = getIP();
    puts(ip);

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
    printf("Accept one connect.");
    IsConnected = 1;
    while (1)
    {
        while(1)
        {
        	Sleep(500);
        	if (Queue[0] != 0)
                if(Queue[0] == 'Q')
                {
                    closesocket(ClientSocket);
                    closesocket(ServeSocket);
                    WSACleanup();
                    return;
                }
                else
                {
                    send(ClientSocket, Queue, 10, 0);
                    Queue[0] = 0;
                    break;
                }
        }
        char recvData[1024] ={0};
        int ret = recv(ClientSocket, recvData, 1024,0);
        if (ret > 0)
        {
            parse_string(recvData, location);
			ChangeInformation(location[0], location[1]);
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
SOCKET init_cli_sock(char *ip);
SOCKET init_cli_sock(char *ip)
{
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
    return sock;
}
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
    	int location[2];
        char recvData[1024]={0};
        int ret = recv(sock, recvData, 1024, 0);
        if (ret > 0)
        {
            parse_string(recvData, location);
			ChangeInformation(location[0], location[1]);
        }

        while (1)
        { 
        	Sleep(500);
            if (Queue[0] != 0)
                if (Queue[0] == 'Q')
                {
                    closesocket(sock);
                    WSACleanup();
                    return;
                }
                else
                {
                    send(sock, Queue, 10, 0);
                    Queue[0] = 0;
                    break;
                }
        }
    }
}

int strlen1(const char * s)
{
	int count=0;
	int i =0;
	while( *(s + i)!= '\0')
	{
		count++;
		i++;
	}
	return count;
	
}
