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
#define INSERT TRUE
#define NOT_INSERT FALSE
#define Start_X 0
const int ms500 = 500;

bool is_display = FALSE;
//cursor status
char text[1000];
int text_index;
//text_index also represents cursor location
double char_width;
//record the width of char which is deleted
double last_char_X[10];
//record every line's last char's x location
int current_line = 0;
//record the current_line
unsigned char draw_char[3] = {'\0','\0','\0'};
//store a character which is to be drown
unsigned char temp_char[3] = {'\0','\0','\0'};
//store a character which is temporary used
void Erase_one_char();
//Erase a right char
void cancelTimer(int timeID);
void ReDraw_text(int is_insert);
void cursor_move(int is_LEFT);
void Draw_one_char(char c);
void Delete_one_char(int is_LEFT);
void Erase_cursor();
void insert_char(char c);
//insert a char into the text array
void get_near_char(int is_LEFT);
void clear_temp_char();

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
    MovePen(Start_X, cy);
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
        // prevent outputing nonsense characters
        //space is not allowed
    else
	{   
        if (text_index == strlen(text))
        {
            Draw_one_char(c);
            text[text_index++] = c;
        }
        else
        {
            insert_char(c);
            if (c <= 127)
                ReDraw_text(INSERT);
        }
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
                        ReDraw_text(NOT_INSERT);
                    }
                    break;
                case VK_DELETE:
                    if (text_index < strlen(text))
                    {
                        Delete_one_char(RIGHT);
                        ReDraw_text(NOT_INSERT);
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
    if (draw_char[0] == '\0')
        draw_char[0] = c;
    else
        draw_char[1] = c;
    if (is_display)
        Erase_cursor();
    if (GetCurrentX() + TextStringWidth(draw_char) > GetWindowWidth())
    {
        last_char_X[current_line++] = GetCurrentX();
        MovePen(Start_X, GetCurrentY() - line_height);
    }
    if (draw_char[0] <= 127 || draw_char[1] != '\0')
    {
        printf("%d\n",draw_char[0]);
        MovePen(GetCurrentX() + TextStringWidth("|"), GetCurrentY());
        DrawTextString(draw_char);
        draw_char[0] ='\0';
        draw_char[1] = '\0';
    }
    
}

void Delete_one_char(int is_LEFT)
{
    if (is_display)
        Erase_cursor();
    SetEraseMode(TRUE);
    if (is_LEFT)
    {
        get_near_char(is_LEFT);
        if (strlen(temp_char) == 2)
        {
            text[--text_index] = '\0';
            text[--text_index] = '\0';
        }
        else
            text[--text_index] = '\0';
        char_width = TextStringWidth(temp_char);
        MovePen(GetCurrentX()-char_width,GetCurrentY());
        //cursor move
        strcat(text, &text[text_index+1]);
        DrawTextString(temp_char);
        MovePen(GetCurrentX()-char_width-TextStringWidth("|"),GetCurrentY());
    }
    else
    {
        get_near_char(is_LEFT);
        if (strlen(temp_char) == 2)
        {
            text[text_index] = '\0';
            text[text_index + 1] = '\0'; 
        }
        else
            text[text_index] = '\0';
        //cursor not move
        char_width = TextStringWidth(temp_char);
        MovePen(GetCurrentX()+TextStringWidth("|"), GetCurrentY());
        strcat(text, &text[text_index+1]);
        DrawTextString(temp_char);
        MovePen(GetCurrentX()-char_width-TextStringWidth("|"),GetCurrentY());
    }
    clear_temp_char();
    SetEraseMode(FALSE);
}
void cursor_move(int is_LEFT)
{
    if (is_display)
        Erase_cursor();
    if (is_LEFT)
    {
        get_near_char(is_LEFT);
        if (strlen(temp_char) == 2)
            text_index -= 2;
        else
            text_index--;
        if (GetCurrentX() -TextStringWidth(temp_char) > 0)
            MovePen(GetCurrentX()-TextStringWidth(temp_char)-TextStringWidth("|"), GetCurrentY());
        else
        {
            MovePen(last_char_X[--current_line], GetCurrentY() + line_height);
            text_index = (temp_char[1] == '\0')?(text_index + 1):(text_index + 2);
            //reset text_index
        }
	}
    else
	{
        get_near_char(is_LEFT);
        if (strlen(temp_char) == 2)
            text_index += 2;
        else
            text_index++;
        if (GetCurrentX() + TextStringWidth(temp_ch) < GetWindowWidth())
            MovePen(GetCurrentX()+TextStringWidth(temp_ch)+TextStringWidth("|"), GetCurrentY());
        else
        {
            MovePen(Start_X, GetCurrentY() - line_height);
            current_line++;
            text_index = (temp_char[1] == '\0')?(text_index - 1):(text_index - 2);
        }
    }
    clear_temp_char();
}

void ReDraw_text(int is_insert)
{
    double original_x,original_y;
    int original_index,original_current_line;
    original_index = text_index;
    original_x = GetCurrentX();
    original_y = GetCurrentY();
    original_current_line = current_line;
    
    if (is_display)
        Erase_cursor();
    cancelTimer(cursor_500);
    //stop the cursor for a while
    if (!is_insert)
        MovePen(original_x + char_width + TextStringWidth("|"), original_y);
    else
    {
        get_near_char(RIGHT);
        text_index += strlen(temp_char);
        clear_temp_char();
    }

    while(text_index < strlen(text))
    {
        Erase_one_char();
        get_near_char(RIGHT);
        text_index += strlen(temp_char);
        clear_temp_char();
    }
    //erase rest string

    MovePen(original_x, original_y);
    //move cursor to original palce
    text_index = original_index;
    current_line = original_current_line;

    while (text_index < strlen(text))
    {
        Draw_one_char(text[text_index]);
        get_near_char(RIGHT);
        text_index += strlen(temp_char);
        clear_temp_char();
    }
    //redraw rest string

    if (!is_insert)
    {
        text_index = original_index;
        MovePen(original_x, original_y);
    }
    else
    {
        text_index = original_index;
        get_near_char(RIGHT);
        MovePen(original_x + TextStringWidth("|") + TextStringWidth(temp_char), original_y);
        text_index += strlen(temp_char)
    }
    current_line = original_current_line;
    clear_temp_char()
    startTimer(cursor_500, ms500);
    //restart cursor
}

void Erase_one_char()
{
    double cx,cy;
    char temp_ch[2] = {text[text_index] , '\0'};
    cx = GetCurrentX();
    cy = GetCurrentY();
    SetEraseMode(TRUE);
    Draw_one_char(text[text_index]);
    SetEraseMode(FALSE);
}
void insert_char(char c)
{
    int index;
    index = strlen(text);
    if (c > 127)
        text[index + 2] = '\0';
    else
        text[index + 1] = '\0';
    while(index > text_index)
    {
        text[index] = text[index - 1];
        index--;
    }
    text[text_index] = c;
}
void get_near_char(int is_LEFT)
{
    if (is_LEFT)
    {
        if (text[text_index - 2] > 127)
        {
            temp_char[0] = text[text_index - 2];
            temp_char[1] = text[text_index - 1];
        }
        else
            temp_char[0] = text[text_index - 1];
    }
    else
    {
        if (text[text_index] > 127)
        {
            temp_char[0] = text[text_index];
            temp_char[1] = text[text_index + 1];
        }
        else
            temp_char =text[text_index];
    }
}
void clear_temp_char()
{
    temp_char[0] = '\0';
    temp_char[1] = '\0';
}