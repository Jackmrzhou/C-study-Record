#include <stdio.h>
void merge_sort(int A[],int p,int r);
void merge(int A[],int p,int q,int r);
int main(void)
{
    int i;
    int array[7] = {2,0,7,9,1,6,3}; 
    merge_sort(array, 0, 6);
    for (i =0; i<7;i++)
        printf("%d ", array[i]);
}
void merge_sort(int A[],int p,int r)
{
    int q;
    if (p < r)
    {
        q = (p + r)/2;
        merge_sort(A,p,q);
        merge_sort(A,q+1,r);
        merge(A,p,q,r);
    }
}
void merge(int A[],int p,int q,int r)
{
    int k;
    int i,j;
    int Blenth =q-p + 2, Clenth = r-q + 1;
    int B[Blenth],C[Clenth];
    i = j = 0;
    for (k = 0;k <= q-p;k++)
        B[k] = A[k+p];
    B[k] = A[r] + 1;
    for (k = 0;k <= r-q-1; k++)
        C[k] = A[q+k+1];
    C[k] = A[q] + 1;
    for (k = p; k <= r;k++)
    {
        if (B[i] <= C[j])
        {
            A[k] = B[i];
            i++;
        }
        else
        {
            A[k] = C[j];
            j++;
        }
    }
}
//O(nlogn)