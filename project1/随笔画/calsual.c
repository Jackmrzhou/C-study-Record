#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "include/graphics.h"
#include "include/extgraph.h"
#include "include/genlib.h"

#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include <conio.h>

#define GAPWIDTH 0.05											/*方框与左边界的距离*/
#define REDHEIGHT 5.15											/*Red框的高度*/
#define YELLOWHEIGHT 4.65										/*Yellow框的高度*/
#define BLUEHEIGHT 4.15											/*Blue框的高度*/
#define TEXTGAP 0.1												/*文字与左边界的距离*/
#define TEXTR 5													/*Red文字的高度*/
#define TEXTY 4.5												/*Yellow文字的高度*/
#define TEXTB 4													/*Blue文字的高度*/
#define REDX1 0.05												/*Red框左x坐标*/
#define REDX2 0.3808											/*Red框右x坐标*/
#define REDY1 4.97												/*Red框上y坐标*/
#define REDY2 5.15												/*Red框下y坐标*/
#define YELLOWX1 0.05											/*Yellow框左x坐标*/
#define YELLOWX2 0.5683											/*Yellow框右x坐标*/
#define YELLOWY1 4.47											/*Yellow框上y坐标*/
#define YELLOWY2 4.65											/*Yellow框下u坐标*/
#define BLUEX1 0.05												/*Blue框左x坐标*/
#define BLUEX2 0.4225											/*Blue框右x坐标*/
#define BLUEY1 3.97												/*Blue框上y坐标*/
#define BLUEY2 4.15												/*Blue框下y坐标*/

#define LENTH2 0.18												/*框的宽*/
#define LENTHOF(a) 0.11 + TextStringWidth(a)					/*框的长*/
void DrawRectangle(double dx, double dy);						/*画长方形函数*/ 
void DrawRed();													/*绘制左上角red选框*/
void DrawYellow();												/*绘制左上角yellow选框*/ 
void DrawBlue();												/*绘制左上角blue选框*/ 
int InRed(double x, double y);									/*判断是否在red选框*/
int InYellow(double x, double y);								/*判断是否在yellow选框*/ 
int InBlue(double x, double y);									/*判断是否在blue选框*/ 
int ChangeColor(double myx, double myy);						/*根据坐标改变颜色*/
void MouseEventProcess(int mouseX, int mouseY, int button, int event);	/*鼠标消息回调函数*/

void Main()
{
	InitGraphics();
	registerMouseEvent(MouseEventProcess);						/*注册鼠标消息回调函数*/
	DrawRed();													/*绘制左上角red选框*/
	DrawYellow();												/*绘制左上角yellow选框*/ 
	DrawBlue();													/*绘制左上角blue选框*/ 

	SetPenColor("Red");											/*默认颜色*/
}

void MouseEventProcess(int x, int y, int button, int event)
{
	static bool IsDown = FALSE;
	static double myx = 0, myy = 0;
	switch(event)
	{
		case BUTTON_DOWN:
			if(IsDown == FALSE)
			{
				myx = ScaleXInches(x);
				myy = GetWindowHeight() - ScaleXInches(y);
				if(ChangeColor(myx, myy) == FALSE)
				{
					IsDown = TRUE;
				}
			}
			break;
		case BUTTON_UP:
			if(IsDown == TRUE)
			{
				IsDown = FALSE;
			}
			break;
		case MOUSEMOVE:
			if(IsDown == TRUE)
			{
				if(myx == 0 && myy == 0)								//*初始情况 
				{														//*
					myx = ScaleXInches(x);								//*
					myy = GetWindowHeight() - ScaleXInches(y);			//*
					MovePen(myx, myy);									//*
				}														//*
				else
				{
					DrawLine(ScaleXInches(x) - myx, GetWindowHeight() -ScaleXInches(y) - myy);	//新坐标减旧坐标  此时Pen移动至鼠标当前位置 
					myx = ScaleXInches(x);														//*更新坐标 
					myy = GetWindowHeight() - ScaleXInches(y);									//* 
				}
			}
			else
			{
				myx = ScaleXInches(x);															//*更新坐标并更新Pen位置 
				myy = GetWindowHeight() - ScaleXInches(y);										//* 
				MovePen(myx, myy);																//* 
			}
			break;
	}
}

void DrawRectangle(double dx, double dy)														//长方形绘制 
{
	DrawLine(dx, 0);
	DrawLine(0, dy);
	DrawLine(-dx, 0);
	DrawLine(0, -dy);
}

void DrawRed()																					/*绘制左上角red选框*/
{
	
	MovePen(GAPWIDTH, REDHEIGHT);
	DrawRectangle(LENTHOF("Red"), -LENTH2);
	MovePen(TEXTGAP, TEXTR);
	DrawTextString("Red");
}

void DrawYellow()																				/*绘制左上角yellow选框*/
{
	MovePen(GAPWIDTH, YELLOWHEIGHT);
	DrawRectangle(LENTHOF("Yellow"), -LENTH2);
	MovePen(TEXTGAP, TEXTY);
	DrawTextString("Yellow");
}

void DrawBlue()																					/*绘制左上角blue选框*/ 
{
	MovePen(GAPWIDTH, BLUEHEIGHT);
	DrawRectangle(LENTHOF("Blue"), -LENTH2);
	MovePen(TEXTGAP, TEXTB);
	DrawTextString("Blue");
}

int InRed(double x, double y)																	/*判断是否在red按钮内*/ 
{
	if( x > REDX1 && x < REDX2 && y > REDY1 && y < REDY2)
		return 1;
	else
		return 0;	
}

int InYellow(double x, double y)																/*判断是否在yellow按钮内*/
{
	if( x > YELLOWX1 && x < YELLOWX2 && y > YELLOWY1 && y < YELLOWY2)
		return 1;
	else
		return 0;	
}

int InBlue(double x, double y)																	/*判断是否在blue按钮内*/
{
	if( x > BLUEX1 && x < BLUEX2 && y > BLUEY1 && y < BLUEY2)
		return 1;
	else
		return 0;	
}

int ChangeColor(double myx, double myy)														/*更改颜色*/ 
{
	if(InRed(myx, myy) == TRUE)
	{
		SetPenColor("Red");
		return TRUE;
	}
	else if(InYellow(myx, myy) == TRUE)
	{
		SetPenColor("Yellow");
		return TRUE;
	}
	else if(InBlue(myx, myy) == TRUE)
	{
		SetPenColor("Blue");
		return TRUE;
	}
	return FALSE;
}
