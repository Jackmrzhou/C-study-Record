#include <stdio.h>
void qsort(int array[], int low, int high);
int partition(int array[], int low, int high);
void swap(int array[],int i, int j);
int main(void)
{
    int test[7]={2,1};
    qsort(test, 0, 1);
    int i;
    for (i = 0; i != 2; ++i)
        printf("%d ", test[i]);
    puts("\n");
    return 0;
}
void qsort(int array[], int low,  int high)
{
    int p;
    if (low < high)
    {
        p = partition(array, low, high);
        qsort(array, low, p - 1);
        qsort(array, p + 1, high);
    }
}
int partition(int array[], int low, int high)
{
    int pivot = array[high];
    int i = low, j = high;
    while (j > i)
    {
        while (array[i] <= pivot && i < j)
            i++;
        while(array[j] >= pivot && j > i)
            j--;
        if (array[i] > array[j])
            swap(array, i ,j);
    }
    swap(array, high, j);
    return j;
}
void swap(int array[], int i, int j)
{
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}