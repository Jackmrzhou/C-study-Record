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

#include <math.h>

#define DiamondLenth 1.0
#define DiamondAngle 60
#define PI 3.14159265

void forward(double distance);
void turn (double angle);
/*The unit is DEG*/
void move(double distance);
double change_angle(double angle);
void DrawDiamond();
void DrawHexagon();
void DrawInside();
void DrawOutline();
double global_angle = 0;

void Main()
{
    double StartX, StartY;
    InitGraphics();
    StartX = GetWindowWidth() / 2;
    StartY = GetWindowHeight() / 2;
    MovePen(StartX, StartY);
    DrawInside();
    MovePen(StartX, StartY);
    DrawOutline();
}

void DrawInside()
{
    int count_diamond,count_repeat;
    for (count_repeat = 0; count_repeat < 3; count_repeat++)
    {
        turn(DiamondAngle / 3);
        for (count_diamond = 0;count_diamond < 6; count_diamond++)
        {
            turn(DiamondAngle);
            DrawDiamond();
        }
    }
    global_angle = 0;
    //reset global_angle
}

void DrawDiamond()
{
    double Now_Angle;
    Now_Angle = global_angle;

    forward(DiamondLenth);
    turn(DiamondAngle);
    forward(DiamondLenth);
    turn(DiamondAngle * 2);
    forward(DiamondLenth);
    turn(DiamondAngle);
    forward(DiamondLenth);

    global_angle = Now_Angle;
    //reset global_angle
}
void DrawHexagon()
{
    int lines;
    double StartX,StartY;
    StartX = GetCurrentX();
    StartY = GetCurrentY();

    move(DiamondLenth * 2);
    turn(DiamondAngle);
    for (lines = 0; lines < 6; lines++)
    {
        turn(DiamondAngle);
        forward(DiamondLenth * 2);
    }
    MovePen(StartX, StartY);
    //reset pen
}

void DrawOutline()
{
    int count_Hexagon;
    for (count_Hexagon = 0; count_Hexagon < 3; count_Hexagon++)
    {
        turn(DiamondAngle / 3);
        DrawHexagon();
    }
}

double change_angle(double angle)
{
    return angle * PI / 180.0;
}
/*
    three functions 
*/
void turn(double angle)
{
    global_angle += angle;
    if (global_angle >= 360)
        global_angle -= 360;
}

void forward(double distance)
{
    DrawLine(distance*cos(change_angle(global_angle)), 
             distance*sin(change_angle(global_angle)));
}

void move(double distance)
{
    MovePen(GetCurrentX() + distance*cos(change_angle(global_angle)), 
            GetCurrentY() + distance*sin(change_angle(global_angle)));
}
