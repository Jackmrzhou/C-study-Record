#include <stdio.h>

int check(int (*map)[10],int x,int y);
int main(void)
{
    int test[10][10]= {
        {0,0,0,0,0,0,0,0,0,0},
        {0,1,1,1,0,0,0,0,0,0},
        {0,1,0,1,1,0,0,0,0,0},
        {0,1,1,0,0,0,0,0,0,0},
        {0,0,0,1,1,0,0,0,0,0},
        {0,1,0,1,1,0,0,0,0,0},
        {0,1,1,0,1,0,0,0,0,0},
        {0,1,1,1,1,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0}
    };
    int result;
    result = check(test,1,0);
    if (result)
        printf("WIN!");
    else
        printf("COUTINUE.");
}
int check(int (*map)[10],int x,int y)
{
    int count=0;
    int i,j;
    i = 9 - y;
    j = 9 - x;
    while(map[i][j] != 0 && i >= 0 && j >= 0 && i < 10 && j < 10)
    {
        i--;
        count++;
    }
    i = 9 - y;
    j = 9 - x;
    while(map[i][j] != 0 && i >= 0 && j >= 0 && i < 10 && j < 10)
    {
        i++;
        count++;
    }
    count--;
    if (count >= 5)
        return 1;
    else
    {
        count = 0;
        i = 9 - y;
        j = 9 - x;
    } 
    while(map[i][j] != 0 && i >= 0 && j >= 0 && i < 10 && j < 10)
    {
        i--;
        j++;
        count++;
    }
    i = 9 - y;
    j = 9 - x;
    while(map[i][j] != 0 && i >= 0 && j >= 0 && i < 10 && j < 10)
    {
        i++;
        j--;
        count++;
    }
    count--;
    if (count >= 5)
        return 1;
    else
        return 0;
}