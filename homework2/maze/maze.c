#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "graphics.h"
#include "genlib.h"
#include "conio.h"

#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "unsize_array.h"
typedef struct STRUCT_MAZE{
    int map[10][10];
    int out_X;
    int out_Y;
}MAZE;
void search_maze(int x,int y,MAZE *maze,link_array *try_path,link_array *success_path);
link_array *write_into_link(int x,int y,link_array *node);
void move(int x,int y,MAZE *maze,link_array *try_path,link_array *success_path);
link_array *copy(link_array *success_path,link_array *try_path);
int count_step(link_array *p);
void Main()
{
    InitGraphics();
    InitConsole();
    MAZE model={.map={
        {0,0,0,0,0,0},
        {0,2,1,1,0,0},
        {0,1,0,1,1,0},
        {0,1,1,0,1,0},
        {0,1,0,1,1,0},
        {0,1,0,1,1,0},
        {0,1,1,0,1,0},
        {0,1,1,1,1,0},
        {0,0,0,0,0,0}
    }
    };
    MAZE *maze;
    maze = (MAZE*)malloc(sizeof(MAZE));
    *maze = model;
    maze->out_X = 7;
    maze->out_Y = 4;
    link_array *success_path,*try_path;
    success_path = create_link();
    try_path = create_link();
    search_maze(1,1,maze,try_path,success_path); 
    if (success_path->next->data[0] == -1)
        printf("NO PATH!");
    else
    {
        print_path(success_path,maze);
        draw(maze, success_path);    
    }
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
    if (maze->map[x][y] == 1)
    {
        maze->map[x][y] = 2;
        try_path = write_into_link(x,y,try_path);
        if ((x) == maze->out_X && y == maze->out_Y)
        {
            maze->map[x][y] = 1;
            success_path = copy(success_path,try_path);
            return;
        }
        else
        {
            search_maze(x, y,maze,try_path, success_path);
            maze->map[x][y] = 1;
        }
    }
}
link_array *write_into_link(int x,int y,link_array *node)
{
    //printf("x=%d,y=%d",x,y);
    link_array *next_node;
    if (node->next->data[0] != -1)
    {
        node->next->data[0] = x;
        node->next->data[1] = y;
        return node->next;
    }
    else
    {
        next_node = (link_array*)malloc(sizeof(link_array));
        next_node->data[0] = x;
        next_node->data[1] = y;
        next_node->next = node->next;
        node->next = next_node;
    }
    return next_node;
}

link_array *copy(link_array *success_path,link_array *try_path)
{
    int out_x = try_path->data[0],out_y = try_path->data[1];
    int success_path_steps, try_path_steps;
    link_array *success_p = success_path;
    //turn to head
    link_array *try_p = try_path->next->next;
    //turn to node after head
    link_array *tansition;
    success_path_steps = count_step(success_path);
    try_path_steps = count_step(try_path);
    if (success_path_steps > try_path_steps || success_path_steps == 0)
    {
        do
        {
            if (success_p->next->data[0] == -1)
            {  
                tansition = (link_array*)malloc(sizeof(link_array));
                tansition->next = success_p->next;
                success_p->next = tansition;
            }
            else 
                tansition = success_p->next;
            tansition->data[0] = try_p->data[0];
            tansition->data[1] = try_p->data[1];
            success_p = success_p->next;
            try_p = try_p->next;
        }
        while(try_p != try_path->next);
    }
    return success_path;//head
}
int count_step(link_array *head)
{
    int step=0;
    link_array *p = head;
    do
    {
        p = p->next;
        step++;
    } 
    while(p != head);
    return step-1;
}
