#ifndef _NET_H
#define _NET_H
#include <winsock2.h>
void Start_Server(void);
void Start_Client(char *ip);
SOCKET init_server_sock();
SOCKET init_cli_sock();
char *getIP();
#endif
