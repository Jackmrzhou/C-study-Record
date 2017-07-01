#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define TB 460
#define NUM 100000
void SortIntegerArray(int a[], int start, int end);
void SelectSort(int a[], int start, int end);
void change(int a[], int x, int y);
int main()
{
	int a[NUM];
	srand(time(NULL));
	FILE *fpr, *fpw;
	fpr = fopen("read", "r");
	int i = 0;
	while (fscanf(fpr, "%d ", &a[i++]) != EOF);
	i--;
	clock_t start, end;
    start = clock();
    SortIntegerArray(a, 0, i-1);
    //SelectSort(a,0,i-1);
    end  = clock();
    clock_t result_1 = end-start;
    start = clock();
    SelectSort(a,0,i-1);
    end = clock();
    clock_t result_2=end-start;
    if (result_2 > result_1)
        printf("%d\n",i);
    //printf("Time:%f\n", (double)(end-start)/CLK_TCK);
    //SortIntegerArray(a, 0 ,i-1);
	fpw = fopen("write", "w");
	for (int j = 0; j < i; j++)
		fprintf(fpw, "%d ", a[j]);
    fclose(fpw);
	return 0;
}

void SortIntegerArray(int a[], int start, int end)
{
	if (end - start <= TB){
		SelectSort(a, start,  end);
	}
	if (end - start == 1)
	{
		if (a[start] > a[end])
			change(a, start, end);
	}
	else if (end - start == 0)
		return;
	else
	{
		int i;
			int *temp = (int *)malloc((end - start + 1) * sizeof(int));
		int mid = (start + end) / 2;
		SortIntegerArray(a, start, mid);
		SortIntegerArray(a, mid + 1, end);
		int ptr1 = start, ptr2 = mid + 1;
		for (i = 0; ptr1 <= mid && ptr2 <= end; i++)
			if (a[ptr1] < a[ptr2])
				temp[i] = a[ptr1++];
			else
				temp[i] = a[ptr2++];
		if (ptr1 <= mid)
			for (; ptr1 <= mid; i++)
				temp[i] = a[ptr1++];
		if (ptr2 <= end)
			for (; ptr2 <= end; i++)
				temp[i] = a[ptr2++];
		for (i = 0; i <= end - start; i++)
			a[start + i] = temp[i];
		free(temp);
	}
}

void SelectSort(int a[], int start, int end)
{
	int i, j;
	int max, index;
	for (i = end; i >= start; i--)
	{
		max = a[start];
		index = start;
		for (j = start; j <= i; j++) {
			if (a[j] > max) {
				max = a[j];
				index = j;
			}
		}
		int temp = a[i];
		a[i] = a[index];
		a[index] = temp;
	}
}

void change(int a[], int x, int y)
{
	int temp = a[x];
	a[x] = a[y];
	a[y] = temp;
}
