#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include "include/graphics.h"
#include "include/extgraph.h"
#include "include/genlib.h"
#include "CAD_TEXT.h"

#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include <conio.h>
#define _MOVE 0
#define _LINE 1
#define _RECTANGLE 2
#define _OVAL 3
#define _TEXT 4
//DrawMode and Type of graphic


typedef struct GRANODE
{
	int TypeOfGra;
	int IsChosen;
	int CanDeleted;
	double GraX1, GraX2, GraY1, GraY2;
//	CAD_TEXT 
	struct GRANODE* next;
}GraNode;

struct
{
	GraNode* head;
	GraNode* tail;
}GraphicNode = {NULL, NULL};

struct
{
	CAD_TEXT* head;
	CAD_TEXT* tail;
}TextLink = {NULL, NULL};

/*typrdef struct
{
	CAD_TEXT *head;
	CAD_TEXT *tail;
}*/

void InsertGra(GraNode* Gra);									//Insert into the node list
void DeleteGra();												//Removed the chosen node from the link list
void InsertTextNode();			

void CharEventProcess(char c);
void KeyboardEventProcess(int key,int event);							
void MouseEventProcess(int x, int y, int button, int event);		
void TimerEventProcess(int timerID);								
//DrawLine has been defined
void DrawOval(double dx, double dy);
void DrawRectangle(double dx, double dy);
void DrawPic(int x, int y, int button, int event);
void (*FucDraw)(double dx, double dy);							//The pointer to the fuction of drawing

void ReDraw();

GraNode *SearchChosen();										
//Search the link list of graphics for chosen one, and then return the pointer to the node
CAD_TEXT *SearchChosenText();
//Search the link list of text for chosen one, and then return the pointer to the node
GraNode *SearchDeleted();			
//Search the link list of graphics for CanDeleted one, and then return the pointer to the node
CAD_TEXT *SearchDeletedText();
//Search the link list of text for CanDeleted one, and then return the pointer to the node

void MoveGra(int x, int y, int button, int event);
//Find out which one is chosen and change the x and y of the chosen node. 
//When event is BUTTON_UP, change the IsChosen of the chosen node to FALSE.
int ChooseGra(GraNode *P, double x,double y);
//Find out which one is chosen and then change the IsChosen of the chosen node to TRUE.
int ChooseLine(GraNode *P, double x,double y);
int ChooseRectangle(GraNode *P, double x,double y);
int ChooseOval(GraNode *P, double x,double y);
int (*FucChoose)(GraNode *P, double x,double y);				//The pointer to the fuction of choosing

double Max(double a, double b);
double Min(double a, double b);


static int DrawMode = _MOVE;									//The mode of draw. Default mode is MOVE.
static bool IsDown = FALSE;										//Whether the button is down.

void PRINT()													//Test Function.(Console)
{
	CAD_TEXT *Q = TextLink.head; 
	GraNode *P = GraphicNode.head;
	while(Q != NULL)
	{
		printf("%s	%d\n", Q->text, Q->CanDeleted);
		Q = Q->next;
	}
	
	while(P != NULL)
	{
		P = P->next;
	}
}
void CharEventProcess(char c)
{
    if(DrawMode == _TEXT)
    {
    	if (c == '\r')
//    {
//        int lenth;
//        lenth = strlen(global_text.text);
//        global_text.text[lenth] = '\0';
//        puts(global_text.text);
//    }
    return;
    else if (c == VK_BACK || c == VK_DELETE || c == VK_SPACE)
        return;
        // prevent outputing nonsense characters
        //space is not allowed
    else
	{   
        if (global_text.text_index == strlen(global_text.text))
        {
        	if (!global_text.is_Chinese && c < 0)
        		global_text.is_Chinese = TRUE;
        	else if(global_text.is_Chinese)
        	{
        		global_text.is_Chinese = FALSE;
        		global_text.record_Chinese_index[global_text.Chinese_counter++] = global_text.text_index + 1;
        	}
            Draw_one_char(c);
            global_text.text[global_text.text_index++] = c;
        }
        else
        {
            if (!global_text.is_Chinese && c < 0)
            //the prototype of function is (char c) so when Chinese the c is negative
            {
                global_text.is_Chinese = TRUE;
                insert_char(c);
            }
            else if (global_text.is_Chinese)
            {
                global_text.is_Chinese = FALSE;
                global_text.record_Chinese_index[global_text.Chinese_counter++] = global_text.text_index + 1;
                insert_char(c);
                global_text.text_index--;
            }
            else
                insert_char(c);
            if (!global_text.is_Chinese)
                ReDraw_text(INSERT);
        }
	}
    }
}
void Main()
{
    InitGraphics();
	InitConsole();
    SetPenColor("Red");
	registerKeyboardEvent(KeyboardEventProcess);		
	registerCharEvent(CharEventProcess);		
	registerMouseEvent(MouseEventProcess);							
}

void MouseEventProcess(int x, int y, int button, int event)
{
	double myx, myy;
	myx = ScaleXInches(x);
	myy = GetWindowHeight() - ScaleXInches(y);
	if(event == BUTTON_DOWN)
		PRINT();
	 
	if(DrawMode == _MOVE)
	{
		MoveGra(x, y, button, event);
	}
	else if(DrawMode == _TEXT)
	{

		if(event == BUTTON_DOWN && button == LEFT_BUTTON)
		{
			if(global_text.Start_X != 0)
			{
				Erase_cursor();
				cancelTimerEvent();
				InsertTextNode();
				Initialize();
			}
			INIT_TEXT(myx, myy);
		}
	}
	else
	{
		DrawPic(x, y, button, event);
	}
}

void KeyboardEventProcess(int key,int event)
{
	if(DrawMode == _TEXT)
    {
    	switch (event)
    	{
        	case KEY_DOWN:
            	switch(key)
            	{
                case VK_BACK:
                    if (global_text.text_index > 0)
                    {
                        Delete_one_char(LEFT);
                        ReDraw_text(NOT_INSERT);
                    }
                    break;
                case VK_DELETE:
                    if (global_text.text_index < strlen(global_text.text))
                    {
                        Delete_one_char(RIGHT);
                        ReDraw_text(NOT_INSERT);
                    }
                    break;
                case VK_LEFT:
                    if (global_text.text_index > 0)
                        cursor_move(LEFT);
                    break;
                case VK_RIGHT:
                    if (global_text.text_index < strlen(global_text.text))
                        cursor_move(RIGHT);
                    break;
                case VK_F1:
                	Erase_cursor();
					cancelTimerEvent();
					InsertTextNode();
					Initialize();
					DrawMode = _MOVE;
					break;
				case VK_F2:
					Erase_cursor();
					cancelTimerEvent();
					InsertTextNode();
					Initialize();
					DrawMode = _LINE;
					break;
				case VK_F3:
				Erase_cursor();
				cancelTimerEvent();
				InsertTextNode();
				Initialize();
				DrawMode = _RECTANGLE;
				break;
			case VK_F4:
				Erase_cursor();
				cancelTimerEvent();
				InsertTextNode();
				Initialize();
				DrawMode = _OVAL;
				break;
			case VK_F5:
				Erase_cursor();
				cancelTimerEvent();
				InsertTextNode();
				Initialize();
				DrawMode = _TEXT;
				break;
            	}
    	}
    }
	
	else
	{
		if(event == KEY_DOWN)
		{
			switch(key)
		{
			case VK_F1:
				DrawMode = _MOVE;
				break;
			case VK_F2:
				DrawMode = _LINE;
				break;
			case VK_F3:
				DrawMode = _RECTANGLE;
				break;
			case VK_F4:
				DrawMode = _OVAL;
				break;
			case VK_F5:
				DrawMode = _TEXT;
				break;
			case VK_DELETE:
				if(event == BUTTON_DOWN)
				{
					SetEraseMode(TRUE);
					ReDraw();
					SetEraseMode(FALSE);
					DeleteGra();
					ReDraw();
				}
		}
		}
	}
}


void DrawRectangle(double dx, double dy)
{
	DrawLine(dx, 0);
	DrawLine(0, dy);
	DrawLine(-dx, 0);
	DrawLine(0, -dy);
}

void DrawOval(double dx, double dy)
{
	DrawEllipticalArc(-dx / 2, -dy / 2, 0.0, 360.0);
}

void DrawPic(int x, int y, int button, int event)
{

	static double myx, myy;
	static GraNode *TempP;
	
	myx = ScaleXInches(x);
	myy = GetWindowHeight() - ScaleXInches(y);
	
	if(IsDown == TRUE)
	{
		SetEraseMode(TRUE);
		ReDraw();
		SetEraseMode(FALSE);
		TempP->GraX2 = myx;
		TempP->GraY2 = myy;
		if(event == BUTTON_UP)
			IsDown = FALSE;
		ReDraw();
	}
	if(IsDown == FALSE && button == LEFT_BUTTON && event == BUTTON_DOWN)
	{
		IsDown = TRUE;
		TempP = (GraNode*)malloc(sizeof(GraNode));
		TempP->TypeOfGra = DrawMode;
		TempP->IsChosen = FALSE;
		TempP->CanDeleted = FALSE;
		TempP->GraX1 = myx;
		TempP->GraY1 = myy;
		TempP->GraX2 = myx;
		TempP->GraY2 = myy;
		TempP->next = NULL;
		InsertGra(TempP);
	}
}

void InsertGra(GraNode* Gra)
{
	if(GraphicNode.head == NULL)
	{
		GraphicNode.head = Gra;
		GraphicNode.tail = Gra;
	}
	else
	{
		GraphicNode.tail->next = Gra;
		GraphicNode.tail = Gra;
	}
	Gra->next = NULL;
}

void DeleteGra()
{
	GraNode *pointG = GraphicNode.head;
	CAD_TEXT *pointT = TextLink.head;
	GraNode *qG;
	CAD_TEXT *qT;
	int IsFoundG = FALSE, IsFoundT = FALSE;
	//Both link lists don't have any node.
	if(GraphicNode.head == NULL && TextLink.head == NULL)
	{
		return;
	}
	//The link list only has one node and it can be deleted.
	else if(GraphicNode.head == NULL && TextLink.head!=NULL && TextLink.head->next != NULL)
	{
		if(pointT->CanDeleted == TRUE)
		{
			TextLink.head = TextLink.head->next;
			free(pointT);
			return;
		}
		else
		{
			qT = TextLink.head->next;
			while(qT != NULL)
			{
				if(qT->CanDeleted == TRUE)
				{
					pointT->next = qT->next;
					free(qT);
					return;
				}
				else
				{
					pointT = qT;
					qT = qT->next;
				}
			}
		}
	}
	else if(TextLink.head == NULL && GraphicNode.head!=NULL && GraphicNode.head->next != NULL)
	{
		if(pointG->CanDeleted == TRUE)
		{
			GraphicNode.head = GraphicNode.head->next;
			free(pointG);
			return;
		}
		else
		{
			qG = GraphicNode.head->next;
			while(qG != NULL)
			{
				if(qG->CanDeleted == TRUE)
				{
					pointG->next = qG->next;
					free(qG);
					return;
				}
				else
				{
					pointG = qG;
					qG = qG->next;
				}
			}
		}
	}
	else if(GraphicNode.head != NULL && GraphicNode.head->next == NULL && GraphicNode.head->CanDeleted == TRUE && TextLink.head == NULL)
	{
		GraphicNode.head = GraphicNode.tail = NULL;
		free(pointG);
		return;
	}
	else if(TextLink.head != NULL && TextLink.head->next == NULL && TextLink.head->CanDeleted == TRUE && GraphicNode.head == NULL)
	{
		TextLink.head = TextLink.tail = NULL;
		free(pointT);
		return;
	}
	//Graphic link list has more than two nodes but the head can be deleted
	else if(TextLink.head!=NULL && TextLink.head->next==NULL && GraphicNode.head!=NULL && GraphicNode.head->next==NULL\
	&& GraphicNode.head->CanDeleted == TRUE)
	{
		GraphicNode.head = GraphicNode.tail = NULL;
		free(pointG);
		return;
	}
	//Text link list has more than two nodes but the head can be deleted
	else if(TextLink.head!=NULL && TextLink.head->next==NULL && GraphicNode.head!=NULL && GraphicNode.head->next==NULL\
	&& TextLink.head->CanDeleted == TRUE)
	{
		TextLink.head = TextLink.tail = NULL;
		free(pointT);
		return;
	}
	//Normal condition
	else if(GraphicNode.head!=NULL && GraphicNode.head->next==NULL && TextLink.head!=NULL && TextLink.head->next!=NULL\
	&& GraphicNode.head->CanDeleted == TRUE)
	{
		GraphicNode.head = GraphicNode.tail = NULL;
		free(pointG);
		return;
	}
	else if(GraphicNode.head!=NULL && GraphicNode.head->next==NULL && TextLink.head!=NULL && TextLink.head->next!=NULL\
	&& GraphicNode.head->CanDeleted != TRUE)
	{
		if(TextLink.head->CanDeleted == TRUE)
		{
			TextLink.head = TextLink.head->next;
			free(pointT);
		}
		else
		{
			qT = TextLink.head->next;
			while(qT != NULL)
			{
				if(qT->CanDeleted == TRUE)
				{
					pointT->next = qT->next;
					free(qT);
					return;
				}
				else
				{
					pointT = qT;
					qT = qT->next;
				}
			}
		}
	}
	else if(TextLink.head!=NULL && TextLink.head->next==NULL && GraphicNode.head!=NULL && GraphicNode.head->next!=NULL\
	&& TextLink.head->CanDeleted == TRUE)
	{
		TextLink.head = TextLink.tail = NULL;
		free(pointT);
		return;
	}
	else if(TextLink.head!=NULL && TextLink.head->next==NULL && GraphicNode.head!=NULL && GraphicNode.head->next!=NULL\
	&& TextLink.head->CanDeleted != TRUE)
	{
		if(GraphicNode.head->CanDeleted == TRUE)
		{
			GraphicNode.head = GraphicNode.head->next;
			free(pointG);
		}
		else
		{
			qG = GraphicNode.head->next;
			while(qG != NULL)
			{
				if(qG->CanDeleted == TRUE)
				{
					pointG->next = qG->next;
					free(qG);
					return;
				}
				else
				{
					pointG = qG;
					qG = qG->next;
				}
			}
		}
	}
	else if(TextLink.head!=NULL && TextLink.head->next!=NULL && GraphicNode.head!=NULL && GraphicNode.head->next!=NULL)
	{
		if(TextLink.head->CanDeleted == TRUE)
		{
			TextLink.head  = TextLink.head->next;
			free(pointT);
			return;
		}
		else if(GraphicNode.head->CanDeleted == TRUE)
		{
			GraphicNode.head = GraphicNode.head->next;
			free(pointG);
			return;
		}
		else
		{
			qT = TextLink.head->next;
			while(qT != NULL)
			{
				if(qT->CanDeleted == TRUE)
				{
					pointT->next = qT->next;
					free(qT);
					return;
				}
				else
				{
					pointT = qT;
					qT = qT->next;
				}
			}
			qG = GraphicNode.head->next;
			while(qG != NULL)
			{
				if(qG->CanDeleted == TRUE)
				{
					pointG->next = qG->next;
					free(qG);
					return;
				}
				else
				{
					pointG = qG;
					qG = qG->next;
				}
			}
		}
	}
}

void InsertTextNode()
{

	CAD_TEXT *P_To_Node;
	P_To_Node = (CAD_TEXT*)malloc(sizeof(CAD_TEXT));
	*P_To_Node = global_text;

	if(TextLink.head == NULL && TextLink.tail == NULL)
	{
		TextLink.head = P_To_Node;
		TextLink.tail = P_To_Node;
	}
	else
	{
		TextLink.tail->next = P_To_Node;
		TextLink.tail = P_To_Node;
	}

}

void ReDraw()
{	
//	PRINT();
	GraNode *pointG = GraphicNode.head;
	CAD_TEXT *pointT = TextLink.head;
	double dx, dy;
	
	if(pointG == NULL && pointT == NULL)
		return;
	else
	{
		while(pointT != NULL)
		{
			Redraw_all(pointT);
			pointT = pointT->next;
		}
		while(pointG != NULL)
		{
			dx = pointG->GraX2 - pointG->GraX1;
			dy = pointG->GraY2 - pointG->GraY1;
			
			switch(pointG->TypeOfGra)
			{
				case _LINE:
					FucDraw = DrawLine;
					MovePen(pointG->GraX1, pointG->GraY1);
					break;
				case _RECTANGLE:
					FucDraw = DrawRectangle;
					MovePen(pointG->GraX1, pointG->GraY1);
					break;
				case _OVAL:
					MovePen(pointG->GraX1, (pointG->GraY1+pointG->GraY2)/2);
					FucDraw = DrawOval;
					break;
				case _TEXT:
					break;
			}
			(*FucDraw)(dx, dy);
			pointG = pointG->next;
		}
	}
}
//******************************************************************	
GraNode *SearchChosen()							   //Search Fuctions			
{															
	GraNode *point = GraphicNode.head;						
	while(point != NULL)									
	{														
		if(point->IsChosen == TRUE)						
		{													
			return point;								
		}													
		point = point->next;									
	}															
	return NULL;												
}																
GraNode *SearchDeleted()									
{																
	GraNode *point = GraphicNode.head;							
	while(point != NULL)										
	{															
		if(point->CanDeleted == TRUE)							 
		{				
			return point;
		}
		point = point->next;
	}
	return NULL;
}
CAD_TEXT *SearchChosenText()
{
	CAD_TEXT *point = TextLink.head;
	while(point != NULL)
	{
		if(point->IsChosen == TRUE)
		{
			return point;
		}
		point = point->next;
	}
	return NULL;
}
CAD_TEXT *SearchDeletedText()
{
	CAD_TEXT *point = TextLink.head;
	while(point != NULL)
	{
		if(point->CanDeleted == TRUE)
		{
			return point;
		}
		point = point->next;
	}
	return NULL;
}
//******************************************************************
void MoveGra(int x, int y, int button, int event)	
{
	static double myx = 0, myy = 0;
	static double myxbe, myybe;
	GraNode *pointG = GraphicNode.head;
	CAD_TEXT *pointT = TextLink.head;
	GraNode *ChoseG;
	CAD_TEXT *ChoseT;

	myxbe = myx;
	myybe = myy;
	myx = ScaleXInches(x);
	myy = GetWindowHeight() - ScaleXInches(y);
	
	if(button == LEFT_BUTTON && event == BUTTON_DOWN && SearchChosenText() == NULL && SearchChosen() == NULL)
	{
		while(pointT != NULL)
		{
			if(ChooseText(pointT, myx, myy) == TRUE)
			{
				ChoseT = SearchChosenText();
				break;
			}
			pointT = pointT->next;
		}
	}
	if(button == LEFT_BUTTON && event == BUTTON_DOWN && SearchChosen() == NULL)
	{
		while(pointG != NULL)
		{
			if(ChooseGra(pointG, myx, myy) == TRUE)
			{
				ChoseG = SearchChosen();
				break;
			}
			pointG = pointG->next;
		}
	}
	if(SearchChosenText() != NULL && event == BUTTON_DOWN)
	{
		ChoseT = SearchChosenText();
		SetEraseMode(TRUE);
		ReDraw();
		SetEraseMode(FALSE);
		ChoseT->Start_X += myx - myxbe;
		ChoseT->Start_Y += myy - myybe;
		ReDraw();
	}
	if(SearchChosenText() != NULL && event == MOUSEMOVE)
	{
		ChoseT = SearchChosenText();
		SetEraseMode(TRUE);
		ReDraw();
		SetEraseMode(FALSE);
		ChoseT->Start_X += myx - myxbe;
		ChoseT->Start_Y += myy - myybe;
		ReDraw();
	}
	else if(SearchChosen() != NULL && event == MOUSEMOVE)
	{
		ChoseG = SearchChosen();
		SetEraseMode(TRUE);
		ReDraw();
		SetEraseMode(FALSE);
		ChoseG->GraX1 += myx - myxbe;
		ChoseG->GraX2 += myx - myxbe;
		ChoseG->GraY1 += myy - myybe;
		ChoseG->GraY2 += myy - myybe;
		ReDraw();
	}
	if(SearchChosenText() != NULL && event == BUTTON_UP)
	{
		ChoseT = SearchChosenText();
		SetEraseMode(TRUE);
		ReDraw();
		SetEraseMode(FALSE);
		ChoseT->Start_X += myx - myxbe;
		ChoseT->Start_Y += myy - myybe;
		ReDraw();
		ChoseT->IsChosen = FALSE;
	}
	if(SearchChosen() != NULL && event == BUTTON_UP)
	{
		ChoseG = SearchChosen();
		SetEraseMode(TRUE);
		ReDraw();
		SetEraseMode(FALSE);
		ChoseG->GraX1 += myx - myxbe;
		ChoseG->GraX2 += myx - myxbe;
		ChoseG->GraY1 += myy - myybe;
		ChoseG->GraY2 += myy - myybe;
		ReDraw();
		ChoseG->IsChosen = FALSE;
	}
}

int ChooseGra(GraNode *P, double x,double y)
{
	switch(P->TypeOfGra)
	{
		case _LINE:
			FucChoose = ChooseLine;
			break;
		case _RECTANGLE:
			FucChoose = ChooseRectangle;
			break;
		case _OVAL:
			FucChoose = ChooseOval;
			break;
		case _TEXT:
			break;
	}
	return (*FucChoose)(P, x, y);
}

int ChooseLine(GraNode *P, double x,double y)
{
	double k, b, d;
	GraNode *pointG = SearchDeleted();
	CAD_TEXT *pointT = SearchDeletedText();

	k = (P->GraY2 - P->GraY1) / (P->GraX2 - P->GraX1);
	b = P->GraY1 - k * P->GraX1;
	d = fabs(k*x + b - y) / sqrt(1+k*k);
	if(x >= Min(P->GraX1, P->GraX2) && x <= Max(P->GraX1, P->GraX2))
	{
		if(y >= Min(P->GraY1, P->GraY2) && y <= Max(P->GraY1, P->GraY2))
		{
			if(d <= 0.1)
			{
				P->IsChosen = TRUE;
				if(pointG != NULL)
				{
					pointG->CanDeleted = FALSE;
				}
				if(pointT != NULL)
				{
					pointT->CanDeleted = FALSE;
				}
				P->CanDeleted = TRUE;
				return 1;
			}
		}
	}
}

int ChooseRectangle(GraNode *P, double x,double y)
{
	GraNode *Dele = SearchDeleted();
	CAD_TEXT *pointT = SearchDeletedText();
	
	if(x >= Min(P->GraX1, P->GraX2) && x <= Max(P->GraX1, P->GraX2))
	{
		if(y >= Min(P->GraY1, P->GraY2) && y <= Max(P->GraY1, P->GraY2))
		{
			P->IsChosen = TRUE;
			if(Dele != NULL)
			{
				Dele->CanDeleted = FALSE;
			}
			if(pointT != NULL)
			{
				pointT->CanDeleted = FALSE;
			}
			P->CanDeleted = TRUE;
			return 1;
		}
	}
}

int ChooseOval(GraNode *P, double x,double y)
{
	double dx, dy;
	double a, b;
	double k;
	GraNode *pointG = SearchDeleted();
	CAD_TEXT *pointT = SearchDeletedText();
	a = fabs((P->GraX1 - P->GraX2) / 2);
	b = fabs((P->GraY1 - P->GraY2) / 2);
	dx = x - (P->GraX1 + P->GraX2) / 2;
	dy = y - (P->GraY1 + P->GraY2) / 2;
	k = (dx*dx)/(a*a) + (dy*dy)/(b*b);
	if(k <= 1)
	{
		P->IsChosen = TRUE;
		if(pointG != NULL)
		{
			pointG->CanDeleted = FALSE;
		}
		if(pointT != NULL)
		{
			pointT->CanDeleted = FALSE;
		}
		P->CanDeleted = TRUE;
		return 1;
	}
	else
	{
		return 0;
	}
}
int ChooseText(CAD_TEXT *global_text, double mouse_x, double mouse_y)
{
	CAD_TEXT *pointT;
    GraNode *pointG;
	pointT = SearchDeletedText();
    pointG = SearchChosen();

    if ((global_text -> Start_Y - global_text -> tail_char_Y) == 0)
        if (mouse_x >= global_text -> Start_X && mouse_x <= global_text -> tail_char_X\
            && (mouse_y <= global_text -> Start_Y + line_height) && mouse_y >= global_text -> Start_Y)
        {
        	if(pointT != NULL)
        	{
        		pointT->CanDeleted = FALSE;
        	}
            if(pointG != NULL)
            {
                pointG->CanDeleted = FALSE;
            }
            global_text->CanDeleted = TRUE;
            global_text->IsChosen = TRUE;
            return TRUE;
        }
        else
        {
        	return FALSE;
        }
	else if (mouse_x >= global_text -> Start_X && mouse_y >= global_text -> tail_char_Y && (mouse_y <= global_text -> Start_Y +line_height))
    {
        global_text->CanDeleted = TRUE;
        global_text->IsChosen = TRUE;
        return TRUE;
    }
    else
    {
    	return FALSE;
    }
            
}

double Max(double a, double b)
{
	return a>b ? a:b;
}

double Min(double a, double b) 
{
	return a<b ? a:b;
}
