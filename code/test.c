#include <stdio.h>
int main()
{
    FILE *fp;
    fp = fopen("students.txt","a+");
    fclose(fp);
    return 0;
}