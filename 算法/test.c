#include  <stdio.h>
void change(int a[]);
int main(void)
{
    int n = 3;
    int a[n];
    int i;
    a[3] =1;
    printf("%d",sizeof(a));
}
void change(int a[])
{
    int temp;
    temp = a[0];
    a[0] = a[1];
    a[1] = temp;
}