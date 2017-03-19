#include <stdio.h>
int main(void)
{
    int A[5] = {1,2,3,4,5};
    int B[5] = {8,1,2,5,5};
    int C[6];
    int i, Lenth, carry;

    for (i = 0; i < 6;i++)
        C[i] = 0;
    //init C
    Lenth = 5;
    carry = 0;
    for (i = Lenth-1;i >= 0;i--)
    {
        if (A[i] + B[i] + carry >= 10)
        {
            C[i+1] = A[i] + B[i] -10 + carry;
            carry = 1;
        }
        else
        {
            C[i+1] = A[i] + B[i] + carry;
            carry = 0;
        }
    }
    if (carry == 1) C[0] = 1;
    //最后是否进位
    for (i = (C[0] == 0);i < Lenth+1;i++)
        printf("%d",C[i]);
}