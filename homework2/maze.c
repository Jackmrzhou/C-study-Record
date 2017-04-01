#include <stdio.h>
#include <stdlib.h>
#include "unsize_array.h"
#define END_X 7
#define END_Y 4
int count_path = 0;
int path[20][50][2];
struct
void search_maze(int x,int y,int (*maze)[6],link_array *node);
void write_into_link(link_array *head);
void move(int x,int y);
int main(void)
{
    int maze[9][6]={
        {0,0,0,0,0,0},
        {0,2,1,1,0,0},
        {0,1,0,1,1,0},
        {0,1,1,0,1,0},
        {0,1,0,1,1,0},
        {0,1,0,1,1,0},
        {0,1,1,0,1,0},
        {0,1,1,1,1,0},
        {0,0,0,0,0,0}
    };
    int depth = 0;
    link_array *array_shortest_head;
    array_shortest_head = create_link();
    search_maze(1,1,maze,head); 
    print_link_array(array_shortest_head);
    if (count_path == 0)
        printf("NO PATH!");
}
void search_maze(int x,int y,int (*maze)[6],link_array *node)
{
    int i;
    for (i = 0;i < 4;i++)
    switch (i)
    {   
        case 0:
            if (maze[x+1][y] == 1)
            {
                move(x+1, y);
                pre = pre->next;
                write_into_link(x+1,y,link_array *pre);
                is_out(x+1, y);
                if ((x+1) == END_X && y == END_Y)
                {
                    maze[x+1][y] = 1;
                    return;
                }
                else
                {
                    search_maze(x+1, y,maze,count_step);
                    maze[x+1][y] = 1;
                }
            }
            break;
        case 1:
            if (maze[x][y+1] == 1)
            {
                maze[x][y+1] = 2;
                path[count_path][count_step][0] = x;
                path[count_path][count_step][1] = y+1;
                count_step++;
                if (x == END_X && (y+1) == END_Y)
                {
                    count_path++;
                    maze[x][y+1] = 1;
                    return;
                }
                else
                {
                    search_maze(x, y+1,maze,count_step);
                    maze[x][y+1] = 1;
                }
            }
            break;
        case 2:
            if (maze[x][y-1] == 1)
            {
                maze[x][y-1] = 2;
                path[count_path][count_step][0] = x;
                path[count_path][count_step][1] = y-1;
                count_step++;
                if (x == END_X && (y-1) == END_Y)
                {
                    count_path++;
                    maze[x][y-1] = 1;
                    return;
                }
                else
                {
                    search_maze(x, y-1,maze,count_step);
                    maze[x][y-1] = 1;
                }
            }
            break;
        case 3:
            if (maze[x-1][y] == 1)
            {
                maze[x-1][y] = 2;
                path[count_path][count_step][0] = x-1;
                path[count_path][count_step][1] = y;
                count_step++;
                if (x-1 == END_X && y == END_Y)
                {
                    count_path++;
                    maze[x-1][y] = 1;
                    return;
                }
                else
                {
                    search_maze(x-1, y,maze,count_step);
                    maze[x-1][y] = 1;
                }
            }
            break;
    }
}

void move(int x,int y, int (*maze)[6])
{
    maze[x][y] = 2;
}
void write_into_link(int x,int y,link_array *node)
{
    node->data[0] = x;
    node->data[1] = y;
    node->next = NULL;
}