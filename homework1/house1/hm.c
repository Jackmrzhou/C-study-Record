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
#define HouseWidth 4.0
#define TriHeight 1.0
#define DoorHeight 1.5
#define DoorWidth 1.0
#define HouseWindowHeight 0.7
#define HouseWindowWidth 0.7
#define StartX 1.5
#define StartY 1.5
#define ChimneyHeight 1.0
#define ChimneyWidth 1.0

void DrawHouse();
void DrawDoor();
void DrawWindow();
void DrawChimney();

void Main()
{
    InitGraphics();
    DrawHouse();
    DrawDoor();
    DrawWindow();
    DrawChimney(); 
}

void DrawHouse()
{
    MovePen(StartX,StartY);
    DrawLine(0.0, HouseHeight);
    DrawLine(HouseWidth, 0.0);
    DrawLine(0.0, -HouseHeight);
    DrawLine(-HouseWidth, 0.0);
    MovePen(StartX, StartY + HouseHeight);
    DrawLine(HouseWidth/2, TriHeight);
    DrawLine(HouseWidth/2, -TriHeight);
}

void DrawDoor()
{
    MovePen(StartX + HouseWidth/2, StartY); 
    DrawLine(0.0, DoorHeight);
    DrawLine(DoorWidth, 0.0);
    DrawLine(0.0, -DoorHeight);
}

void DrawWindow()
{
    MovePen(StartX + 0.5,StartY + DoorHeight);
    DrawLine(HouseWindowWidth, 0.0);
    DrawLine(0.0, -HouseWindowHeight);
    DrawLine(-HouseWindowWidth, 0.0);
    DrawLine(0.0, HouseWindowHeight);
}

void DrawChimney()
{ 
    MovePen(StartX + 0.5, StartY + HouseHeight + 0.25);
    DrawLine(0.0, ChimneyHeight);
    MovePen(StartX + 1.0, StartY + HouseHeight + 0.5);
    DrawLine(0.0, ChimneyHeight - 0.25);
    MovePen(StartX + 0.25, StartY + HouseHeight + 0.25 + ChimneyHeight);
    DrawLine(ChimneyWidth, 0.0);
    DrawLine(0.0, 0.4);
    DrawLine(-ChimneyWidth, 0.0);
    DrawLine(0.0, -0.4);
}
