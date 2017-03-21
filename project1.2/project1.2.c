#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "graphics.h"
#include "genlib.h"
#include "conio.h"
#include "extgraph.h"
#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include <string.h>

#define cursor_500 1
#define line_height 0.2
#define LEFT TRUE
#define RIGHT FALSE
const int ms500 = 500;

bool is_display = FALSE;
//cursor status
char text[1000];
int text_index;
//text_index also represent cursor location
double char_width;
double last_char_X[10];
//record every line's last char's x location
int line = 0;
//record lines
bool exit_loop = FALSE;
int count=0;

void Erase_one_char();
//Erase a right char
void cancelTimer(int timeID);
void ReDraw_text(int is_LEFT);
void cursor_move(int is_LEFT);
void Draw_one_char(char c);
void Delete_one_char(int is_LEFT);
void Erase_cursor();

void CharEventProcess(char c);
void keyboardEventProcess(int key,int event);
void TimerEventProcess(int timerID);
void startTimer(int timerID, int timeinterval);

void Main()
{
    double cy;
    InitConsole();
    InitGraphics();
    cy = GetWindowHeight()/2;
    MovePen(0, cy);
    registerCharEvent(CharEventProcess);
    registerKeyboardEvent(keyboardEventProcess);
    registerTimerEvent(TimerEventProcess);
    startTimer(cursor_500, ms500);
}
void CharEventProcess(char c)
{
    if (c == '\r')
    {
        int lenth;
        lenth = strlen(text);
        text[lenth] = '\0';
        puts(text);
    }
    else if (c == VK_BACK || c == VK_DELETE || c == VK_SPACE)
        return;
        /*prevent outputing nonsense characters*/
    else
	{ 
        Draw_one_char(c);
        text[text_index++] = c;
	}
}
void keyboardEventProcess(int key,int event)
{
    switch (event)
    {
        case KEY_DOWN:
            switch(key)
            {
                case VK_BACK:
                    if (text_index > 0)
                    {
                        Delete_one_char(LEFT);
                        ReDraw_text(RIGHT);
                    }
                    break;
                case VK_DELETE:
                    if (text_index < strlen(text))
                    {
                        Delete_one_char(RIGHT);
                        ReDraw_text(LEFT);
                    }
                    break;
                case VK_LEFT:
                    if (text_index > 0)
                        cursor_move(LEFT);
                    break;
                case VK_RIGHT:
                    if (text_index < strlen(text))
                        cursor_move(RIGHT);
                    break;
            }
    }
}

void TimerEventProcess(int timerID)
{
    switch(timerID)
    {
        case cursor_500:
        {
            if (!is_display)
            {
                SetEraseMode(FALSE);
                DrawTextString("|");
                MovePen(GetCurrentX()-TextStringWidth("|"),GetCurrentY());
                is_display = !is_display;
            }
            else
            {
                SetEraseMode(TRUE);
                DrawTextString("|");
                MovePen(GetCurrentX()-TextStringWidth("|"),GetCurrentY());
                SetEraseMode(FALSE);
                is_display = !is_display;
            }
        }break;

    }
}
void Erase_cursor()
{
    SetEraseMode(TRUE);
    DrawTextString("|"); 
    MovePen(GetCurrentX()-TextStringWidth("|"),GetCurrentY());
    SetEraseMode(FALSE);
    is_display = FALSE;
}

void Draw_one_char(char c)
{
    char s[2] = {c, '\0'};
    if (is_display)
        Erase_cursor();
    if (GetCurrentX() + TextStringWidth(s) > GetWindowWidth())
    {
        last_char_X[line++] = GetCurrentX();
        MovePen(0, GetCurrentY() - line_height);
    }
    MovePen(GetCurrentX() + TextStringWidth("|"), GetCurrentY());
    DrawTextString(s);
}

void Delete_one_char(int is_LEFT)
{
    if (is_display)
        Erase_cursor();
    SetEraseMode(TRUE);
    if (is_LEFT)
    {
        char ch_LEFT[2] = {text[text_index - 1], '\0'};
        char_width = TextStringWidth(ch_LEFT);
        MovePen(GetCurrentX()-char_width,GetCurrentY());
        text[--text_index] = '\0';
        //cursor move
        strcat(text, &text[text_index+1]);
        DrawTextString(ch_LEFT);
        MovePen(GetCurrentX()-char_width-TextStringWidth("|"),GetCurrentY());
    }
    else
    {
        char ch_RIGHT[2] = {text[text_index], '\0'};
        char_width = TextStringWidth(ch_RIGHT);
        MovePen(GetCurrentX()+TextStringWidth("|"), GetCurrentY());
        text[text_index] = '\0';
        //cursor not move
        strcat(text, &text[text_index+1]);
        DrawTextString(ch_RIGHT);
        MovePen(GetCurrentX()-char_width-TextStringWidth("|"),GetCurrentY());
    }
    
    SetEraseMode(FALSE);
}
void cursor_move(int is_LEFT)
{
    if (is_display)
        Erase_cursor();
    if (is_LEFT)
    {
        text_index--;
        char temp_ch[2] = {text[text_index], '\0'};
        if (GetCurrentX() -TextStringWidth(temp_ch) > 0)
            MovePen(GetCurrentX()-TextStringWidth(temp_ch)-TextStringWidth("|"), GetCurrentY());
        else
        {
            MovePen(last_char_X[--line], GetCurrentY() + line_height);
            text_index++;
        }
	}
    else
	{
        text_index++;
        char temp_ch[2] = {text[text_index-1], '\0'};
        if (GetCurrentX() + TextStringWidth(temp_ch) < GetWindowWidth())
            MovePen(GetCurrentX()+TextStringWidth(temp_ch)+TextStringWidth("|"), GetCurrentY());
        else
        {
            MovePen(0, GetCurrentY() - line_height);
            line++;
            text_index--;
        }
    }
}

void ReDraw_text(int is_LEFT)
{
    double original_x,original_y;
    int original_index,original_line;
    original_index = text_index;
    original_x = GetCurrentX();
    original_y = GetCurrentY();
    original_line = line;
    if (is_display)
        Erase_cursor();
    cancelTimer(cursor_500);
    //stop the cursor for a while
    while(text_index < strlen(text) +count)
    {
        Erase_one_char();
        Draw_one_char(text[text_index]);
        //Must erase firstly or what you are drawing may cover other characters
        text_index++;
        if (exit_loop)
            break;
    }
    MovePen(original_x, original_y);
    //move cursor to original palce
    text_index = original_index;
    line = original_line;
    startTimer(cursor_500, ms500);
    //restart cursor
    if (exit_loop)
        exit_loop = !exit_loop;

}

void Erase_one_char()
{
    double cx,cy;
    char temp_ch[2] = {text[text_index] , '\0'};
    cx = GetCurrentX();
    cy = GetCurrentY();
    SetEraseMode(TRUE);
    if (cx + char_width + TextStringWidth("|") < GetWindowWidth() 
        && cx + TextStringWidth("|") + TextStringWidth(temp_ch) < GetWindowWidth())
        MovePen(cx + char_width + TextStringWidth("|"), cy);
    else if(cx + char_width + TextStringWidth("|") < GetWindowWidth())
    {
        exit_loop = TRUE;
        count++;
    }
    else
    {
        MovePen(char_width + TextStringWidth("|"), cy - line_height);
        cx = 0;
        cy = cy - line_height;
    }
    Draw_one_char(text[text_index]);
    MovePen(cx, cy);
    SetEraseMode(FALSE);
}
