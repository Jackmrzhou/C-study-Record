#include <stdio.h>
int RemoveZeroElements(int *array, int size);
int main(void)
{
    int test_array[6]={1,0,3,5,0,7};
    int size=6;
    size = RemoveZeroElements(test_array, size);
    int i;
    for (i = 0; i != size; ++i)
        printf("%d ", test_array[i]);
    printf("%d\n", size);
    return 0;
}
int RemoveZeroElements(int *array,int size)
{
    int i;
    for (i = 0; i <= size; ++i)
    {
        if (array[i] == 0)
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