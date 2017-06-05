#include <stdio.h>
int RemoveDuplicates(int *array, int size);
int main(void)
{
    int test_array[8] = {1,1,2,3,4,4,5,5};
    int size=8;
    size = RemoveDuplicates(test_array, size);
    int i;
    for (i=0; i != size; ++i)
        printf("%d ",test_array[i]);
    printf("\n%d\n", size);
    return 0;
}
int RemoveDuplicates(int *array,int size)
{
    int i;
    for (i = 1; i <= size; ++i)
    {
        if (array[i]==array[i-1])
        {
            int temp=i;
            while(temp++ <= size -2)
                array[temp-1] = array[temp];
            size--;
        }
        else
            continue;
    }
    return size;
}