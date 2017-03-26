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
const int ms500 = 500;

typedef struct CAD_Text{
    bool is_Chinese;
    unsigned char text[1000];
    //store text
    unsigned char temp_char[3];
    //store a character which is temporary used
    unsigned char draw_char[3];
    //store a character which is to be drown
    int record_Chinese_index[500];
    int Chinese_counter;
    int text_index;
    //text_index also represents cursor location
    int current_line;
    //record the current_line
    double char_width;
    //record the width of char which is deleted
    double last_char_X[10];
    //record every line's last char's x location
    double Start_X;
    double Start_Y;
    //Start_X and Start_Y are where the text start
    double tail_char_Y;
    double tail_char_X;
    struct CAD_Text* next;
}CAD_TEXT;

bool is_display = FALSE;
//cursor status

void Erase_one_char();
//Erase a right char
void cancelTimer(int timeID);
void ReDraw_text(int is_insert);
void cursor_move(int is_LEFT);
void Draw_one_char(unsigned char c);
void Delete_one_char(int is_LEFT);
void Erase_cursor();
void insert_char(unsigned char c);
//insert a char into the text array
void get_near_char(int is_LEFT);
void clear_temp_char();
void delete_Chinese_index();
void Redraw_all();
void is_chosen(double mouse_x,double mouse_y);

void CharEventProcess(char c);
void keyboardEventProcess(int key,int event);
void TimerEventProcess(int timerID);
void startTimer(int timerID, int timeinterval);

CAD_TEXT global_text={FALSE,
                        {0},
                        {'\0', '\0', '\0'},
                        {'\0', '\0', '\0'},
                        {0},
                        0,
                        0,
                        0,
                        0,
                        {0},
                        0,
                        0,
                        0,
                        0};
void Main()
{
    InitGraphics();
    MovePen(Start_X, Start_Y);
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
        lenth = strlen(global_text.text);
        global_text.text[lenth] = '\0';
        puts(global_text.text);
    }
    else if (c == VK_BACK || c == VK_DELETE || c == VK_SPACE)
        return;
        // prevent outputing nonsense characters
        //space is not allowed
    else
	{   
        if (global_text.text_index == strlen(global_text.text))
        {
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
void keyboardEventProcess(int key,int event)
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

void Draw_one_char(unsigned char c)
{
    if (global_text.draw_char[0] == '\0')
        global_text.draw_char[0] = c;
    else
        global_text.draw_char[1] = c;
    if (is_display)
        Erase_cursor();
    if (GetCurrentX() + TextStringWidth(global_text.draw_char) > GetWindowWidth())
    {
        global_text.last_char_X[global_text.current_line++] = GetCurrentX();
        MovePen(Start_X, GetCurrentY() - line_height);
    }
    if (global_text.draw_char[0] <= 127 || global_text.draw_char[1] != '\0')
    {
        MovePen(GetCurrentX() + TextStringWidth("|"), GetCurrentY());
        DrawTextString(global_text.draw_char);
        global_text.draw_char[0] ='\0';
        global_text.draw_char[1] = '\0';
        global_text.tail_char_Y = GetCurrentY();
        global_text.tail_char_X = GetCurrentY();
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
        if (strlen(global_text.temp_char) == 2)
        {
            global_text.text[--global_text.text_index] = '\0';
            global_text.text[--global_text.text_index] = '\0';
            strcat(global_text.text, &global_text.text[global_text.text_index+2]);
            delete_Chinese_index();
        }
        else
        {
            global_text.text[--global_text.text_index] = '\0';
            strcat(global_text.text, &global_text.text[global_text.text_index+1]);
        }
        //cursor move
        global_text.char_width = TextStringWidth(global_text.temp_char);
        if (GetCurrentX() - global_text.char_width < Start_X)
            MovePen(global_text.last_char_X[--global_text.current_line], GetCurrentY() + line_height);
        MovePen(GetCurrentX()-global_text.char_width,GetCurrentY());
        DrawTextString(global_text.temp_char);
        MovePen(GetCurrentX()-global_text.char_width-TextStringWidth("|"),GetCurrentY());
    }
    else
    {
        get_near_char(is_LEFT);
        if (strlen(global_text.temp_char) == 2)
        {
            global_text.text[global_text.text_index] = '\0';
            global_text.text[global_text.text_index + 1] = '\0';
            strcat(global_text.text, &global_text.text[global_text.text_index+2]); 
        }
        else
        {
            global_text.text[global_text.text_index] = '\0';
            strcat(global_text.text, &global_text.text[global_text.text_index+1]);
        }
        //cursor not move
        global_text.char_width = TextStringWidth(global_text.temp_char);
        MovePen(GetCurrentX()+TextStringWidth("|"), GetCurrentY());
        DrawTextString(global_text.temp_char);
        MovePen(GetCurrentX()-global_text.char_width-TextStringWidth("|"),GetCurrentY());
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
        if (strlen(global_text.temp_char) == 2)
            global_text.text_index -= 2;
        else
            global_text.text_index--;
        if (GetCurrentX() -TextStringWidth(global_text.temp_char) > 0)
            MovePen(GetCurrentX()-TextStringWidth(global_text.temp_char)-TextStringWidth("|"), GetCurrentY());
        else
        {
            MovePen(global_text.last_char_X[--global_text.current_line], GetCurrentY() + line_height);
            global_text.text_index = (global_text.temp_char[1] == '\0')?(global_text.text_index + 1):(global_text.text_index + 2);
            //reset text_index
        }
	}
    else
	{
        get_near_char(is_LEFT);
        if (strlen(global_text.temp_char) == 2)
            global_text.text_index += 2;
        else
            global_text.text_index++;
        if (GetCurrentX() + TextStringWidth(global_text.temp_char) < GetWindowWidth())
            MovePen(GetCurrentX()+TextStringWidth(global_text.temp_char)+TextStringWidth("|"), GetCurrentY());
        else
        {
            MovePen(Start_X, GetCurrentY() - line_height);
            global_text.current_line++;
            global_text.text_index = (global_text.temp_char[1] == '\0')?(global_text.text_index - 1):(global_text.text_index - 2);
        }
    }
    clear_temp_char();
}

void ReDraw_text(int is_insert)
{
    double original_x,original_y;
    int original_index,original_current_line;
    original_index = global_text.text_index;
    original_x = GetCurrentX();
    original_y = GetCurrentY();
    original_current_line = global_text.current_line;
    
    if (is_display)
        Erase_cursor();
    cancelTimer(cursor_500);
    //stop the cursor for a while
    if (!is_insert)
        MovePen(original_x + global_text.char_width + TextStringWidth("|"), original_y);
    else
    {
        get_near_char(RIGHT);
        global_text.text_index += strlen(global_text.temp_char);
        clear_temp_char();
    }

    while(global_text.text_index < strlen(global_text.text))
    {
        Erase_one_char();
        global_text.text_index++;
    }
    //erase rest string

    MovePen(original_x, original_y);
    //move cursor to original palce
    global_text.text_index = original_index;
    global_text.current_line = original_current_line;

    while (global_text.text_index < strlen(global_text.text))
    {
        
        Draw_one_char(global_text.text[global_text.text_index]);
        global_text.text_index++;
    }
    //redraw rest string

    if (!is_insert)
    {
        global_text.text_index = original_index;
        MovePen(original_x, original_y);
    }
    else
    {
        global_text.text_index = original_index;
        get_near_char(RIGHT);
        MovePen(original_x + TextStringWidth("|") + TextStringWidth(global_text.temp_char), original_y);
        global_text.text_index += strlen(global_text.temp_char);
    }
    global_text.current_line = original_current_line;
    clear_temp_char();
    startTimer(cursor_500, ms500);
    //restart cursor
}

void Erase_one_char()
{
    double cx,cy;
    cx = GetCurrentX();
    cy = GetCurrentY();
    SetEraseMode(TRUE);
    Draw_one_char(global_text.text[global_text.text_index]);
    SetEraseMode(FALSE);
}
void insert_char(unsigned char c)
{
    int index;
    index = strlen(global_text.text);
    global_text.text[index + 1] = '\0'; 
    while(index > global_text.text_index)
    {
        global_text.text[index] = global_text.text[index - 1];
        index--;
    }
    global_text.text[global_text.text_index] = c;
    if (global_text.is_Chinese)
        global_text.text_index++;  
}
void get_near_char(int is_LEFT)
{  
    int loop;
    if (is_LEFT)
    {
        for (loop = 0;loop < global_text.Chinese_counter;loop++)
        {
            if (global_text.text_index == global_text.record_Chinese_index[loop])
            {
                global_text.temp_char[0] = global_text.text[global_text.text_index - 2];
                global_text.temp_char[1] = global_text.text[global_text.text_index - 1];
                break;
            }
        }
        if (strlen(global_text.temp_char) != 2)
            global_text.temp_char[0] = global_text.text[global_text.text_index - 1];
    }
    else
    {
        if (global_text.text[global_text.text_index] > 127)
        {
            global_text.temp_char[0] = global_text.text[global_text.text_index];
            global_text.temp_char[1] = global_text.text[global_text.text_index + 1];
        }
        else
            global_text.temp_char[0] =global_text.text[global_text.text_index];
    }
}
void clear_temp_char()
{
    global_text.temp_char[0] = '\0';
    global_text.temp_char[1] = '\0';
}

void delete_Chinese_index()
{
    int index;
    for (index = 0; index < global_text.Chinese_counter; index++)
    {
        if (global_text.record_Chinese_index[index] == global_text.text_index + 2)
            break;
    }
    while(index < global_text.Chinese_counter)
    {
        global_text.record_Chinese_index[index] = global_text.record_Chinese_index[index + 1];
        index++;
    }
    global_text.Chinese_counter--;
}

void Redraw_all()
{
    int index = 0;
    double original_x,original_y;
    original_x = GetCurrentX();
    original_y = GetCurrentY();
    //record current x,y
    MovePen(Start_X, Start_Y);
    SetEraseMode(TRUE);
    while (index < strlen(global_text.text_index))
        Draw_one_char(global_text.text[index]);
    SetEraseMode(FALSE);
    index = 0;
    MovePen(Start_X, Start_Y);
    while (index < strlen(global_text.text_index))
        Draw_one_char(global_text.text[index]);
    MovePen(original_x, original_y);
    //reset x,y
}

void is_chosen(CAD_TEXT *global_text,double mouse_x, ,double mouse_y)
{
    if ((mouse_x < GetWindowWidth() && mouse_x >= Start_X) 
        && (mouse_y <= (Start_Y + line_height) && mouse_y >= global_text.tail_char_Y ))
        if ((mouse_x > global_text.tail_char_X  && mouse_y < global_text.tail_char_Y + line_height))
            return FALSE;
        else
            return TRUE;
}