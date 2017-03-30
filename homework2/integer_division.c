#include <stdio.h>
int result[100];
int number;
void division(int num,int index);
int main(void)
{
    int index;
    scanf("%d",&number);
    division(number,0);
    return 0;
}
void division(int num,int index)
{
    int i;
    if (num == 0 && index != 1)
    {
        printf("%d=",number); 
        for (i =0; i < index - 1;i++)
            printf("%d+",result[i]);
        printf("%d\n", result[i]);
    }
    for (i = num;i > 0; i--)
    {
        if (index == 0 || i <= result[index - 1])
        {
            result[index] = i;
            division(num - i,index + 1);
        }
    }
}
