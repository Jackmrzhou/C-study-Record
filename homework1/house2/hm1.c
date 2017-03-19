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

#define HouseHeight 2.0
#define HouseWidth 3.0
#define SHeight 0.2
#define TriHeight  1.0 

void DrawRoof();
void DrawTwoFloors();
void DrawLittleRectangle();

void Main()
{
	InitGraphics();
    DrawTwoFloors();
    DrawRoof();
    DrawLittleRectangle();
}
void DrawTwoFloors()
{
    int i, Floor;
    double RectangleWidth;
    RectangleWidth = HouseWidth/4;
    for (Floor = 0; Floor < 2;Floor++){ 
        for (i = 0; i < 4;i++)
        {
            MovePen(i*RectangleWidth, Floor*HouseHeight/2);
            DrawLine(RectangleWidth, 0.0);
            DrawLine(0.0, HouseHeight / 2);
            DrawLine(-RectangleWidth, 0.0);
            DrawLine(0.0, -HouseHeight / 2);
        }
    }
}

void DrawRoof()
{
    int i;
    double RectangleWidth;
    RectangleWidth = HouseWidth/4;
    for (i = 0;i < 2;i++)
    {
        MovePen(i*RectangleWidth*3,HouseHeight);
        DrawLine(RectangleWidth/2, TriHeight);
        DrawLine(RectangleWidth/2, -TriHeight);
    }
}

void DrawLittleRectangle()
{
    int i;
    double LittleRectangleWidth;
    LittleRectangleWidth = HouseWidth / 24;
    for (i = 0; i < 6;i++)
    {
        MovePen(HouseWidth/4 + i*LittleRectangleWidth*2,HouseHeight);
        DrawLine(0.0, SHeight);
        DrawLine(LittleRectangleWidth, 0.0);
        DrawLine(0.0, -SHeight);
    } 
}
