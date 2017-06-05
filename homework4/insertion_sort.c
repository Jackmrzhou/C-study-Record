#include <stdio.h>
#define Lenth 6
int main(void)
{
    int A[Lenth] = {6, 4, 1, 3, 5, 2};
    int B[Lenth] = {6, 4, 1, 3, 5, 2};
    //A,B for test
    int key, i, j;
    for (i = 1;i < Lenth;i++)
    {
        key = A[i];
        j = i -1;
        while(j >= 0 && key < A[j])
        {
            A[j + 1] = A[j];
            j--;
        }
        A[j+1] = key;
    }
    //increase edition
    for (i = Lenth - 2; i >= 0;i--)
    {
        key = B[i];
        j = i + 1;
        while (j < Lenth && key < B[j])
        {
            B[j - 1] = B[j];
            j++;
        }
        B[j -1 ] = key;
    }
    //decrease edition
    for (i = 0; i<Lenth; i++)
        printf("%d ",A[i]);
    for (i = 0; i < Lenth;i++)
        printf("%d ",B[i]);
    return 0;
}
//O(n*n)