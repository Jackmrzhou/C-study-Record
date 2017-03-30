#ifndef _CAD_TEXT_H
#define _CAD_TEXT_H

#define cursor_500 1
#define line_height 0.2
#define LEFT TRUE
#define RIGHT FALSE
#define INSERT TRUE
#define NOT_INSERT FALSE

extern bool is_display;
//cursor status
extern const int ms500;
//cursor status



typedef struct CAD_Text{
    bool is_Chinese;
    bool IsChosen;
    bool CanDeleted;
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
    double tail_char_X;
    double tail_char_Y;
    //Start_X and Start_Y are where the text start
    struct CAD_Text* next;
}CAD_TEXT;
void Erase_one_char();
//Erase a right char
void cancelTimer(int timeID);
//stop timer
void ReDraw_text(int is_insert);
//redraw the text after the cursor
void cursor_move(int is_LEFT);
//move cursor
void Draw_one_char(unsigned char c);
//draw one character on the screen
void Delete_one_char(int is_LEFT);
//delete one character from the text
void Erase_cursor();
//erase the current cursor
void insert_char(unsigned char c);
//insert a char into the text array
void get_near_char(int is_LEFT);
//get the left/right character and store in the temp_char
void clear_temp_char();
//clear the char store in temp_char
void delete_Chinese_index();
//delete one specific Chinese index record 
void Redraw_all();
//redraw all text
int is_chosen(CAD_TEXT * global_text, double mouse_x, double mouse_y);
//judge is chosen

extern CAD_TEXT global_text;
//global_text is the current text which is being operating

#endif
