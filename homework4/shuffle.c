#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef struct {
    int value;
    int priority;
}Node;
Node list[52];
void select_sort(Node a[],int n);
int main(void)
{
    int i;
    srand((unsigned)time(NULL));
    for (i = 0;i < 52; ++i)
    {
        list[i].value = i+1;
        list[i].priority = rand();
    }
    select_sort(list,52);
    for (i = 0; i < 52;++i)
        printf("%d ", list[i].value);
    puts("\n");
    return 0;
}
void select_sort(Node a[],int n)
{
    int i,j,min;
    Node t;
    for(i=0;i<n-1;i++)
    {
        min=i;
        for(j=i+1;j<n;j++)
            if(a[min].priority>a[j].priority)
                min=j;
        if(min!=i)
        {
            t=a[min];
            a[min]=a[i];
            a[i]=t;
        }
    }
}
