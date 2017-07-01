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

#define GAPWIDTH 0.05											/*��������߽�ľ���*/
#define REDHEIGHT 5.15											/*Red��ĸ߶�*/
#define YELLOWHEIGHT 4.65										/*Yellow��ĸ߶�*/
#define BLUEHEIGHT 4.15											/*Blue��ĸ߶�*/
#define TEXTGAP 0.1												/*��������߽�ľ���*/
#define TEXTR 5													/*Red���ֵĸ߶�*/
#define TEXTY 4.5												/*Yellow���ֵĸ߶�*/
#define TEXTB 4													/*Blue���ֵĸ߶�*/
#define REDX1 0.05												/*Red����x����*/
#define REDX2 0.3808											/*Red����x����*/
#define REDY1 4.97												/*Red����y����*/
#define REDY2 5.15												/*Red����y����*/
#define YELLOWX1 0.05											/*Yellow����x����*/
#define YELLOWX2 0.5683											/*Yellow����x����*/
#define YELLOWY1 4.47											/*Yellow����y����*/
#define YELLOWY2 4.65											/*Yellow����u����*/
#define BLUEX1 0.05												/*Blue����x����*/
#define BLUEX2 0.4225											/*Blue����x����*/
#define BLUEY1 3.97												/*Blue����y����*/
#define BLUEY2 4.15												/*Blue����y����*/

#define LENTH2 0.18												/*��Ŀ�*/
#define LENTHOF(a) 0.11 + TextStringWidth(a)					/*��ĳ�*/
void DrawRectangle(double dx, double dy);						/*�������κ���*/ 
void DrawRed();													/*�������Ͻ�redѡ��*/
void DrawYellow();												/*�������Ͻ�yellowѡ��*/ 
void DrawBlue();												/*�������Ͻ�blueѡ��*/ 
int InRed(double x, double y);									/*�ж��Ƿ���redѡ��*/
int InYellow(double x, double y);								/*�ж��Ƿ���yellowѡ��*/ 
int InBlue(double x, double y);									/*�ж��Ƿ���blueѡ��*/ 
int ChangeColor(double myx, double myy);						/*��������ı���ɫ*/
void MouseEventProcess(int mouseX, int mouseY, int button, int event);	/*�����Ϣ�ص�����*/

void Main()
{
	InitGraphics();
	registerMouseEvent(MouseEventProcess);						/*ע�������Ϣ�ص�����*/
	DrawRed();													/*�������Ͻ�redѡ��*/
	DrawYellow();												/*�������Ͻ�yellowѡ��*/ 
	DrawBlue();													/*�������Ͻ�blueѡ��*/ 

	SetPenColor("Red");											/*Ĭ����ɫ*/
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
				if(myx == 0 && myy == 0)								//*��ʼ��� 
				{														//*
					myx = ScaleXInches(x);								//*
					myy = GetWindowHeight() - ScaleXInches(y);			//*
					MovePen(myx, myy);									//*
				}														//*
				else
				{
					DrawLine(ScaleXInches(x) - myx, GetWindowHeight() -ScaleXInches(y) - myy);	//�������������  ��ʱPen�ƶ�����굱ǰλ�� 
					myx = ScaleXInches(x);														//*�������� 
					myy = GetWindowHeight() - ScaleXInches(y);									//* 
				}
			}
			else
			{
				myx = ScaleXInches(x);															//*�������겢����Penλ�� 
				myy = GetWindowHeight() - ScaleXInches(y);										//* 
				MovePen(myx, myy);																//* 
			}
			break;
	}
}

void DrawRectangle(double dx, double dy)														//�����λ��� 
{
	DrawLine(dx, 0);
	DrawLine(0, dy);
	DrawLine(-dx, 0);
	DrawLine(0, -dy);
}

void DrawRed()																					/*�������Ͻ�redѡ��*/
{
	
	MovePen(GAPWIDTH, REDHEIGHT);
	DrawRectangle(LENTHOF("Red"), -LENTH2);
	MovePen(TEXTGAP, TEXTR);
	DrawTextString("Red");
}

void DrawYellow()																				/*�������Ͻ�yellowѡ��*/
{
	MovePen(GAPWIDTH, YELLOWHEIGHT);
	DrawRectangle(LENTHOF("Yellow"), -LENTH2);
	MovePen(TEXTGAP, TEXTY);
	DrawTextString("Yellow");
}

void DrawBlue()																					/*�������Ͻ�blueѡ��*/ 
{
	MovePen(GAPWIDTH, BLUEHEIGHT);
	DrawRectangle(LENTHOF("Blue"), -LENTH2);
	MovePen(TEXTGAP, TEXTB);
	DrawTextString("Blue");
}

int InRed(double x, double y)																	/*�ж��Ƿ���red��ť��*/ 
{
	if( x > REDX1 && x < REDX2 && y > REDY1 && y < REDY2)
		return 1;
	else
		return 0;	
}

int InYellow(double x, double y)																/*�ж��Ƿ���yellow��ť��*/
{
	if( x > YELLOWX1 && x < YELLOWX2 && y > YELLOWY1 && y < YELLOWY2)
		return 1;
	else
		return 0;	
}

int InBlue(double x, double y)																	/*�ж��Ƿ���blue��ť��*/
{
	if( x > BLUEX1 && x < BLUEX2 && y > BLUEY1 && y < BLUEY2)
		return 1;
	else
		return 0;	
}

int ChangeColor(double myx, double myy)														/*������ɫ*/ 
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
