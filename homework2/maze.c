#include <stdio.h>
#include <stdlib.h>
int count_path = 0;
int path[10][10][2];
void search_maze(int x,int y,int (*maze)[5],int count_step);
int main(void)
{
    int maze[5][5]={
        {0,0,0,0,0},
        {0,1,1,0,0},
        {0,1,0,1,0},
        {0,1,1,1,0},
        {0,0,0,0,0}
    };
    int count_step = 0;
    search_maze(1,1,maze,count_step); 
    int i,j;
    for (i = 0;i < count_path;i++)
    {
        for (j = 0; j < 10; j++)
            printf("(%d,%d)",path[count_path][j][0],path[count_path][j][1]);
        printf("\n");
    }
}
void search_maze(int x,int y,int (*maze)[5],int count_step)
{
    int i,flag = 0;
    for (i = 0;i < 4;i++)
    switch (i)
    {   
        case 0:
            if (maze[x+1][y])
            {
                flag = 1;
                maze[x+1][y] = 2;
                path[count_path][count_step][0] = x+1;
                path[count_path][count_step][1] = y;
                count_step++;
                if ((x+1) == 3 && y == 3)
                {
                    count_path++;
                    return;
                }
                else
                    search_maze(x+1, y,maze,count_step);
            }
            break;
        case 1:
            if (maze[x][y+1])
            {
                flag = 1;
                maze[x][y+1] = 2;
                path[count_path][count_step][0] = x;
                path[count_path][count_step][1] = y+1;
                count_step++;
                if (x == 3 && (y+1) == 3)
                {
                    count_path++;
                    return;
                }
                else
                    search_maze(x, y+1,maze,count_step);
            }
            break;
        case 2:
            if (maze[x][y-1])
            {
                flag = 1;
                maze[x][y-1] = 2;
                path[count_path][count_step][0] = x;
                path[count_path][count_step][1] = y-1;
                count_step++;
                if (x == 3 && (y-1) == 3)
                {
                    count_path++;
                    return;
                }
                else
                    search_maze(x, y-1,maze,count_step);
            }
            break;
        case 3:
            if (maze[x-1][y])
            {
                flag = 1;
                maze[x-1][y] = 2;
                path[count_path][count_step][0] = x-1;
                path[count_path][count_step][1] = y;
                count_step++;
                if (x-1 == 3 && y == 3)
                {
                    count_path++;
                    return;
                }
                else
                    search_maze(x-1, y,maze,count_step);
            }
            break;
    }
    if (!flag)
    {
        count_step--;
        maze[x][y] = 0;
    }
}