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
#include "maze.h"
#include "unsize_array.h"

#define Rec_Height 0.4
#define Rec_Width 0.4
#define Start_X 0
void Draw_maze(MAZE *maze);
void Draw_Rec(int is_block);
void Draw_path(link_array* success_path,double IN_X,double IN_Y);
void draw(MAZE *maze,link_array *success_path)
{
    double Start_Y;
    Start_Y = GetWindowHeight();
    MovePen(Start_X, Start_Y);
    Draw_maze(maze);
    double IN_X,IN_Y;
    IN_X = Start_X + Rec_Width;
    IN_Y = Start_Y - Rec_Height; 
    Draw_path(success_path, IN_X, IN_Y);
}

void Draw_maze(MAZE *maze)
{
    int i,j;
    for (i = 0; i < 10;i++)
    {
        for (j = 0;j < 10;j++)
            Draw_Rec(maze->map[i][j]);        
        MovePen(Start_X, GetCurrentY() - Rec_Height);
    }
}

void Draw_path(link_array * success_path, double IN_X, double IN_Y)
{
    int direction[2];
    link_array *p = success_path;
    MovePen(IN_X + Rec_Width/4*3, IN_Y - Rec_Height/2);
    DrawArc(Rec_Height/4, 0,360);
    //draw start
    do{
        if (p == success_path)
        {
            direction[0] = p->next->data[0] - 1;
            direction[1] = p->next->data[1] - 1;
        }
        else
        {
            direction[0] = p->next->data[0] - p->data[0];
            direction[1] = p->next->data[1] - p->data[1];
        }
        p = p->next;
        MovePen(GetCurrentX() + Rec_Width * direction[1],
                GetCurrentY() - Rec_Height * direction[0]);
        DrawArc(Rec_Height/4, 0,360);
    }
    while (p->next != success_path);
}

void Draw_Rec(int is_block)
{
    DrawLine(Rec_Width, 0);
    DrawLine(0, -Rec_Height);
    DrawLine(-Rec_Width, 0);
    DrawLine(0, Rec_Height);
    if(!is_block)
    //0 represents block
    {
        DrawLine(Rec_Width, -Rec_Height);
        MovePen(GetCurrentX(), GetCurrentY() + Rec_Height);
        DrawLine(-Rec_Width, -Rec_Height);
        MovePen(GetCurrentX() + Rec_Width, GetCurrentY() + Rec_Height);
    }
    else
        MovePen(GetCurrentX() + Rec_Width, GetCurrentY());
}
