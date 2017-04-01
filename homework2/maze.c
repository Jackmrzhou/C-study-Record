#include <stdio.h>
#include <stdlib.h>
#include "unsize_array.h"
typedef struct STRUCT_MAZE{
    int map[10][10];
    int out_X;
    int out_Y;
}MAZE;
int count_path = 0;
int path[20][50][2];
void search_maze(int x,int y,MAZE *maze,link_array *try_path,link_array *success_path);
void write_into_link(link_array *success_path,link_array *try_path);
void move(int x,int y,MAZE *maze,link_array *try_path,link_array *success_path);
int main(void)
{
    MAZE maze;
    maze->map={
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
    maze->out_X = 7;
    maze->out_Y = 4;
    link_array *success_path,*try_path;
    success_path = create_link();
    search_maze(1,1,maze,try_path,success_path); 
    
    if (success_path->next->data[0] == -1)
        printf("NO PATH!");
    else
        print_link_array(success_path);
}
void search_maze(int x,int y,MAZE *maze,link_array *try_path,link_array *success_path)
{
    int i;
    for (i = 0;i < 4;i++)
    switch (i)
    {   
        case 0:
            move(x+1,y,maze,try_path,success_path);
            break;
        case 1:
            move(x,y+1,maze,try_path,success_path);
            break;
        case 2:
            move(x,y-1,maze,try_path,success_path);
            break;
        case 3:
            move(x-1,y,maze,try_path,success_path);
            break;
    }
}

void move(int x,int y,MAZE *maze,link_array *try_path,link_array *success_path)
{ 
    if (maze[x][y] == 1)
    {
        maze[x][y] = 2;
        move(x, y);
        try_path = write_into_link(x,y,link_array *try_path);
        if ((x) == maze->out_X && y == maze->out_Y)
        {
            maze[x][y] = 1;
            copy(link_array *success_path,link_array *try_path);
            return;
        }
        else
        {
            search_maze(x, y,maze,try_path, success_path);
            maze[x][y] = 1;
        }
    }
}
void write_into_link(int x,int y,link_array *node)
{
    link_array *next_node;
    next_node->data[0] = x;
    next_node->data[1] = y;
    node->next = next_node;
    next_node->next = node->next;
    return next_node;
}

void copy(link_array *success_path,link_array *try_path)
{
    link_array *success_p = success_path;
    link_array *try_p = try_path;
    while(try_p->next != try_path)
    {
        *success_p = *try_path;
        success_p->next = success_p;
        success_p->next = success_path;
        try_p = try_p->next;
    }
    return success_p->next;
}