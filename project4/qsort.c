#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define MAX_LENTH 100000
void quick_sort(int array[], int low, int high);
int partition(int array[], int low, int high);
void swap(int array[],int i, int j);
int main(void)
{
    int test[MAX_LENTH];
    FILE *fpr,*fpw;
    fpr = fopen("read", "r");
    int len = 0;
	while (fscanf(fpr, "%d ", &test[len++]) != EOF);
    len -= 1;
    clock_t start, end;
    start = clock();
    quick_sort(test, 0, len-1);
    end  = clock();
    printf("Time:%f\n", (double)(end-start)/CLK_TCK);
    fpw = fopen("write", "w");
    int i;
    for (i = 0; i != len; ++i)
        fprintf(fpw,"%d ", test[i]);
    fclose(fpw);
    return 0;
}
void quick_sort(int array[], int low,  int high)
{
    int p;
    if (low < high)
    {
        p = partition(array, low, high);
        quick_sort(array, low, p - 1);
        quick_sort(array, p + 1, high);
    }
}
int partition(int array[], int low, int high)
{
    int random=rand()%(high - low)+ low;
    swap(array,random,high);
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