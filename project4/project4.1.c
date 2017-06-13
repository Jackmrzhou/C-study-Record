#include <stdio.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define NUM 100000
void qsort(int a[], int head, int tail);
void swap(int a[], int x, int y);
int main()
{
	int a[NUM];
	FILE *fpr, *fpw;
	fpr = fopen("read.txt", "r");
	int i = 0;
	while (fscanf(fpr, "%d ", &a[i++]) != EOF);
	i--;
	qsort(a, 0, i-1);
	fpw = fopen("write.txt", "w");
	int j;
	for (j = 0; j < i; j++)
		fprintf(fpw, "%d ", a[j]);
	return 0;
}
void qsort(int a[], int head, int tail){
	if (tail - head == 1){
		if (a[head] > a[tail]) {
			swap(a, head, tail);
		}
		return;
	}
	int pivot = a[head], current_ptr = 1;
	int ptr[2] = { head, tail }, add[2] = { 1, -1 }, bigger[2] = {1, 0};
	while (ptr[0] != ptr[1]){
		if ((a[ptr[current_ptr]] > pivot) == bigger[current_ptr]){
			a[ptr[!current_ptr]] = a[ptr[current_ptr]];
			current_ptr = !current_ptr;
			ptr[current_ptr]+=add[current_ptr];
		}
		else
			ptr[current_ptr] += add[current_ptr];
	}
	a[ptr[0]] = pivot;
	qsort(a, head, ptr[0] - 1), qsort(a, ptr[0] + 1, tail);
}
void swap(int a[], int x, int y){
	int temp = a[x];
	a[x] = a[y];
	a[y] = temp;
}