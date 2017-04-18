#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <windows.h>
#include "Net.h"
char *Queue = NULL;
char *Get_Data = NULL;
void* NetThread_1(void* args);
void* NetThread_2(void *args);
int Start_New_Thread(int is_Server)
{
    //char * ip ="127.0.0.1";
    if (is_Server)
    {    
        pthread_t t1;
        pthread_create(&t1, NULL, NetThread_1, NULL);
    }
    else
    {
        pthread_t t2;
        puts("Please input remote IP:");
        scanf("%s",ip);
        pthread_create(&t2, NULL, NetThread_2, ip);
    }
}
void* NetThread_1(void* args)
{
    Start_Server();
    return NULL;
}
void* NetThread_2(void* args)
{
    char *ip = args;
    Start_Client(ip);
    return NULL;
}