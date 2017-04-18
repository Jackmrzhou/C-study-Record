#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "include/graphics.h"
#include "include/extgraph.h"
#include "include/genlib.h"
#include "Net.h"
#include "NetThread.h"
#include <pthread.h>

#include <winsock2.h>

#pragma comment (lib, "ws2_32.lib")

#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include <conio.h>

#define NONE -1
#define START 0
#define PLAY 2
#define EXIT 3
#define ONLINE 4
#define SERVER 1
#define CLIENT 2
#define WIN 3
#define MAPSIZE 19
#define PLAYCHESS 1
#define TAKEBACK 2
#define BLACK 0
#define WHITE 1
#define RESTART 1

#define PLAY_X 				4.0
#define PLAY_Y  			4.0
#define EXIT_X              4.0
#define EXIT_Y              2.0
#define TITLE_X             4.0
#define TITLE_Y             5.0
#define PENSIZE_OF_HEAD     5.0
#define CHESSBOARD_START_X  1.0
#define CHESSBOARD_START_Y  6.0
#define LENTH_OF_SQR        0.25
#define PLAYER_PEN_SIZE     5.0
#define PLAYER1_X			8.0
#define PLAYER1_Y			5.0
#define PLAYER2_X  			8.0
#define PLAYER2_Y			3.0
#define TAKEBACK_PEN_SIZE	4.0
#define TAKEBACK_X			8.0
#define TAKEBACK_X2			(8+TextStringWidth("悔棋"))
#define TAKEBACK_Y			4.0
#define TAKEBACK_Y2			4.5
#define YOUWIN_PEN_SIZE		10.0
#define YOUWIN_X			4.0
#define YOUWIN_Y			4.0
#define WINSTEPS_X			3.8 
#define WINSTEPS_Y			3.0
#define RESTART_PEN_SIZE	4.0
#define RESTART_X			4.4 
#define RESTART_X2 			(4.4+TextStringWidth("再来一局"))
#define RESTART_Y			2.0
#define RESTART_Y2          2.5
#define WIN_EXIT_X			4.55 
#define WIN_EXIT_X2			(4.55+TextStringWidth("退出"))
#define WIN_EXIT_Y			1.0
#define WIN_EXIT_Y2         1.5
#define STARTSCREEN_PLAY_X1 4.0
#define STARTSCREEN_PLAY_X2 (4+TextStringWidth("开始游戏"))
#define STARTSCREEN_PLAY_Y1 4.0
#define STARTSCREEN_PLAY_Y2 4.5
#define STARTSCREEN_ONLINE_X1 4.0
#define STARTSCREEN_ONLINE_X2 (4.0+TextStringWidth("线上对战"))
#define STARTSCREEN_ONLINE_Y1 3.0
#define STARTSCREEN_ONLINE_Y2 3.5 
#define STARTSCREEN_EXIT_X1 4.0
#define STARTSCREEN_EXIT_X2 (4+TextStringWidth("退出"))
#define STARTSCREEN_EXIT_Y1 2.0
#define STARTSCREEN_EXIT_Y2 2.5
#define ONLINE_SCREEN_SERVER_X1 4.0
#define ONLINE_SCREEN_SERVER_X2 (4.0+TextStringWidth("Server"))
#define ONLINE_SCREEN_SERVER_Y1 5.0
#define ONLINE_SCREEN_SERVER_Y2 5.5
#define ONLINE_SCREEN_CLIENT_X1 4.0
#define ONLINE_SCREEN_CLIENT_X2 (4.0+TextStringWidth("Client"))
#define ONLINE_SCREEN_CLIENT_Y1 3.0
#define ONLINE_SCREEN_CLIENT_Y2 3.5


typedef struct
{
	int color;
	int Record[180][2];
	int IsWinner;
	int index;
}PLAYER;

void MouseEventProcess(int x, int y, int button, int event);
void parse_string(char *str,int *location);
void DrawStartScreen();
void DrawChessBoard();
void DrawOnlineChoice();
void DrawWinScreen(PLAYER Winner);
void DrawPiece(PLAYER piece);
void ChangeInformation(int x, int y);
void CleanChessBoard(double Center_X, double Center_Y);
void ReDrawMap(PLAYER player[]);
void DrawRec(double x, double y);
void Judge(int (*map)[MAPSIZE], PLAYER* player);
void parse_string(char *str, int * location);

int JudgeXY(int x, int y, int *location);
int CheckStartChoice(int x, int y);
int CheckOnlineChoice(int x, int y);
int ChangePlayer(int Current_player);
int CheckWinChoice(int x, int y);

int map[MAPSIZE][MAPSIZE];
int IsConnected;
char Queue[10];
int CanDrop;

int Winner = -1;
int PlayChoose;
int WinChoice = NONE;
int Current_index = 0;
int Mode = START;
int Current_player = BLACK;
int location[2];
int My_Player, Your_Player;
int IsOnline = FALSE;
PLAYER player[2] = 	{
					{BLACK, {0,0}, FALSE, -1},
					{WHITE, {0,0}, FALSE, -1}
					};
SOCKET your_sock;
void Main()
{
  
	int i, j;
	InitGraphics();
	InitConsole();
	DefineColor("SPECIALCOLOR", 213.0/255, 176.0/255, 146.0/255);
	DefineColor("RICE", 1, 1, 231.0/255);
	registerMouseEvent(MouseEventProcess);
	for(i = 0; i < MAPSIZE; i++)
		for(j = 0; j < MAPSIZE; j++)
			map[i][j] = -1;
	IsConnected = FALSE;
	DrawStartScreen();
}

void MouseEventProcess(int x, int y, int button, int event)
{
	int i, j;
	
	static int Online_Choice = NONE;
	static int StartChoice;
	
	switch(Mode)
	{
		case START:
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
			{
				StartChoice = CheckStartChoice(x, y);
				switch(StartChoice)
				{
					case NONE:
						break;
					case PLAY:
						Mode = PLAY;
						IsOnline = FALSE;
						CanDrop = TRUE;
						SetEraseMode(TRUE);
						DrawStartScreen();
						SetEraseMode(FALSE);
						DrawChessBoard();
						break;
					case ONLINE:
						Mode = ONLINE;
						IsOnline = TRUE;
						SetEraseMode(TRUE);
						DrawStartScreen();
						SetEraseMode(FALSE);
						DrawOnlineChoice();
						break;
					case EXIT:
						Pause(1);
						ExitGraphics();
						break;
				}
			}
			break;
		case PLAY:
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
			{
				PlayChoose = JudgeXY(x, y, location);
				if(PlayChoose == PLAYCHESS && CanDrop == TRUE)
				{
					if(IsOnline)
						Current_player = My_Player;
					player[Current_player].Record[Current_index][0] = location[0];
					player[Current_player].Record[Current_index][1] = location[1];
					player[Current_player].index = Current_index;
					Judge(map, &player[Current_player]);
					DrawPiece(player[Current_player]);
					/*SetEraseMode(TRUE);
					MovePen(0, 7);
					StartFilledRegion(1);
					DrawRec(10, 7);
					EndFilledRegion();
					SetEraseMode(FALSE);
					DrawChessBoard();
					ReDrawMap(player);*/ 
				}
				else if(PlayChoose == TAKEBACK)
				{
					if(Current_index == 0 && Current_player == BLACK)
						break;

					if ((IsOnline == TRUE && CanDrop == FALSE) || IsOnline == FALSE)
					{
						SetEraseMode(TRUE);
						MovePen(0, 7);
						StartFilledRegion(1);
						DrawRec(10, 7);
						EndFilledRegion();
						SetEraseMode(FALSE);
						if(Current_player == BLACK)
							Current_index--;
						Current_player = ChangePlayer(Current_player);
						map[player[Current_player].Record[Current_index][0]][player[Current_player].Record[Current_index][1]] = -1;
						player[Current_player].index = Current_index-1;
						DrawChessBoard();
						ReDrawMap(player);
					}
				}
			}
			else if(button == LEFT_BUTTON && event == BUTTON_UP && CanDrop == TRUE)					//????,????
			{
				if(PlayChoose == PLAYCHESS)
				{
					if(player[Current_player].IsWinner == TRUE)
					{
						sprintf(Queue, "X%d,%d", x, y);
						send(your_sock, Queue, 10, 0);
						SetEraseMode(TRUE);
						DrawChessBoard();
						MovePen(0, 7);
						StartFilledRegion(1);
						DrawRec(10, 7);
						EndFilledRegion();
						SetEraseMode(FALSE);
						Winner = Current_player;
						DrawWinScreen(player[Winner]);
						Winner = Current_player;
						Mode = WIN;
					}
					else
					{
						sprintf(Queue, "X%d,%d", x, y);
						send(your_sock, Queue, 10, 0);
                    	Queue[0] = 0;
						if(Current_player == WHITE)
							Current_index++;
						Current_player = ChangePlayer(Current_player);
						if(IsOnline)
							CanDrop = FALSE;
						char recvData[32] ={0};
						int loca[2];
        				int ret = recv(your_sock, recvData, 32,0);
        				if (ret > 0)
        				{
            				parse_string(recvData, loca);
							ChangeInformation(loca[0], loca[1]);
        				}
        				
					}
				}
				else if(PlayChoose == TAKEBACK)
				{
					if(IsOnline && CanDrop == FALSE)
						CanDrop = TRUE;
				}
			}
			break;
		case ONLINE:
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
			{
				Online_Choice = CheckOnlineChoice(x, y);
				SetEraseMode(TRUE);
				DrawOnlineChoice();
				SetEraseMode(FALSE);
				MovePen(ONLINE_SCREEN_CLIENT_X1, ONLINE_SCREEN_CLIENT_Y1);
				DrawTextString("Waiting for connected...");
				UpdateDisplay();
				if(Online_Choice == SERVER)
				{
					My_Player = BLACK;
					Your_Player = WHITE;
					
							SetEraseMode(TRUE);
							MovePen(ONLINE_SCREEN_CLIENT_X1, ONLINE_SCREEN_CLIENT_Y1);
							DrawTextString("Waiting for connected...");
							SetEraseMode(FALSE);
							DrawChessBoard();
							Mode = PLAY;
							CanDrop = TRUE;
							your_sock = init_server_sock();
							break;
							
						
					
				}
				else if(Online_Choice == CLIENT)
				{
					My_Player = WHITE;
					Your_Player = BLACK;
					
					
							SetEraseMode(TRUE);
							MovePen(ONLINE_SCREEN_CLIENT_X1, ONLINE_SCREEN_CLIENT_Y1);
							DrawTextString("Waiting for connected...");
							SetEraseMode(FALSE);
							DrawChessBoard();
							Mode = PLAY;
							CanDrop = FALSE;
							char ip[10]; 
   							puts("Please input remote IP:");
    						scanf("%s",ip);
							your_sock = init_cli_sock(ip);
							UpdateDisplay();
							char recvData[32] ={0};
						int loca[2];
        				int ret = recv(your_sock, recvData, 32,0);
        				if (ret > 0)
        				{
            				parse_string(recvData, loca);
							ChangeInformation(loca[0], loca[1]);
						}
							break;
						
					
				}
				break;
			}
			break;
		case WIN:
			if(event == BUTTON_DOWN && button == LEFT_BUTTON)
			{
				WinChoice = CheckWinChoice(x, y);
				if(WinChoice == RESTART)
				{
					SetEraseMode(TRUE);
					DrawWinScreen(player[Winner]);
					SetEraseMode(FALSE);
					DrawChessBoard();
					Mode = PLAY;
					Winner = -1;
					Current_index = -1;
					Current_player = WHITE;
					for(i = 0; i < MAPSIZE; i++)
						for(j = 0; j < MAPSIZE; j++)
							map[i][j] = -1;

					player[0].color = BLACK, player[0].Record[0][0] = 0, player[0].Record[0][1] = 0, player[0].IsWinner = FALSE, player[0].index = -1;
					player[1].color = WHITE, player[1].Record[0][0] = 0, player[1].Record[0][1] = 0, player[1].IsWinner = FALSE, player[1].index = -1;
				}
				else if(WinChoice == EXIT)
				{
					Pause(3);
					ExitGraphics();
				}
			}
			break;
	}
}

void Judge(int (*map)[MAPSIZE], PLAYER *player)
{
    int x,y,count = 0;
    int i,j;
    int index = player->index;
    x = player->Record[index][0];
    y = player->Record[index][1];

    map[x][y] = player->color;

    for (i = 0;i < 2;i++)
        for (j = 0;j < 2;j++)
        {
            if (i == 0 && j == 0)
                continue;
            x = player->Record[index][0];
    		y = player->Record[index][1];
            while (map[x][y] == player->color && x>=0 && x < MAPSIZE
            && y >= 0 && y < MAPSIZE)
            {
                x += i;
                y += j;
                count++;
            }
            x = player->Record[index][0];
            y = player->Record[index][1];
            while (map[x][y] == player->color && x>=0 && x < MAPSIZE
            && y >= 0 && y < MAPSIZE)
            {
                x -= i;
                y -= j;
                count++;
            }
            if (count > 5)
            {
                player->IsWinner = 1;
                return;
            }
            count = 0;
        }
    
    i = 1;
    j = -1;
    //the remain condition

    while (map[x][y] == player->color && x >= 0 && x < MAPSIZE
    && y >= 0 && y < MAPSIZE)
    {
        x += i;
        y += j;
        count++;
    }
    x = player->Record[index][0];
    y = player->Record[index][1];
    while (map[x][y] == player->color && x >= 0 && x < MAPSIZE
    && y >= 0 && y < MAPSIZE)
    {
        x -= i;
        y -= j;
        count++;
    }
    if (count-1 >= 5)
    {
        player->IsWinner = 1;
        return;
    }
}


void DrawStartScreen()
{
	MovePen(PLAY_X, PLAY_Y);
	DrawTextString("开始游戏");
	MovePen(STARTSCREEN_ONLINE_X1, STARTSCREEN_ONLINE_Y1);
	DrawTextString("线上对战");
	MovePen(EXIT_X, EXIT_Y);
	DrawTextString("退出");
	MovePen(TITLE_X, TITLE_Y);
	if(GetEraseMode() == FALSE)
	{
		SetPenColor("BLUE");
	}
	DrawTextString("五子棋");
}

void DrawChessBoard()
{
	int i, j;
	SetPenColor("YELLOW");
	MovePen(0, 7);
	StartFilledRegion(1);
	DrawRec(10, 7);
	EndFilledRegion();
	SetPenColor("SPECIALCOLOR");
	MovePen(CHESSBOARD_START_X, CHESSBOARD_START_Y);
	StartFilledRegion(1);
	DrawRec((MAPSIZE-1)*LENTH_OF_SQR, (MAPSIZE-1)*LENTH_OF_SQR);
	EndFilledRegion();
	SetPenColor("BLACK");
	for(i = 0; i < MAPSIZE-1; i++)
	{
		for(j = 0; j < MAPSIZE-1; j++)
		{
			MovePen(CHESSBOARD_START_X + j * LENTH_OF_SQR, CHESSBOARD_START_Y - i * LENTH_OF_SQR);
			DrawRec(LENTH_OF_SQR, LENTH_OF_SQR);
		}
	}


	MovePen(PLAYER1_X, PLAYER1_Y);
	DrawTextString("玩家1 黑");
	MovePen(PLAYER2_X, PLAYER2_Y);
	DrawTextString("玩家2 白");
	MovePen(TAKEBACK_X, TAKEBACK_Y);
	DrawTextString("悔棋");
}

void DrawOnlineChoice()
{
	MovePen(ONLINE_SCREEN_SERVER_X1, ONLINE_SCREEN_SERVER_Y1);
	DrawTextString("Server");
	MovePen(ONLINE_SCREEN_CLIENT_X1, ONLINE_SCREEN_CLIENT_Y1);
	DrawTextString("Client");
}

void DrawWinScreen(PLAYER Winner)
{
	int power = 10;
	int i;
	int steps = Winner.index+1;
	int temp;
	char Congratulation1[50];
	char Congratulation2[50];
	sprintf(Congratulation1, "恭喜你!玩家%d!你赢了!", Winner.color+1);
	sprintf(Congratulation2, "你只用了%d步就击败了对方!", Winner.index+1);
	if(IsOnline == FALSE || Winner.color == My_Player)
	{
		MovePen(YOUWIN_X, YOUWIN_Y);
		DrawTextString(Congratulation1);
		MovePen(WINSTEPS_X, WINSTEPS_Y);
		DrawTextString(Congratulation2);
		MovePen(RESTART_X, RESTART_Y);
		DrawTextString("再来一局");
		MovePen(WIN_EXIT_X, WIN_EXIT_Y);
		DrawTextString("退出");
	}
	else
	{
		printf("in online fail\n");
		SetPenColor("BLACK");
		MovePen(RESTART_X, RESTART_Y);
		DrawTextString("很遗憾！您输了！");
		printf("Drawn one");
		MovePen(WIN_EXIT_X, WIN_EXIT_Y);
		printf("color = %s, erase = %d\n", GetPenColor(), GetEraseMode());
		DrawTextString("退出");
		printf("Drawn two");
	}
}

void DrawPiece(PLAYER piece)
{
	double Right_X, Center_Y;
	int x = piece.Record[piece.index][0], y = piece.Record[piece.index][1];
	
	Right_X = CHESSBOARD_START_X + y * LENTH_OF_SQR + LENTH_OF_SQR/2;
	Center_Y = CHESSBOARD_START_Y - x * LENTH_OF_SQR;
	
	CleanChessBoard(Right_X, Center_Y);
	MovePen(Right_X, Center_Y);
	StartFilledRegion(1);
	if(piece.color == BLACK)
		SetPenColor("BLACK");
	else
		SetPenColor("RICE");
	DrawArc(LENTH_OF_SQR/2, 0, 360);

	EndFilledRegion();


}

void ReDrawMap(PLAYER player[])
{
	int i;
	int x, y;
	double Right_X, Center_Y;
	
	for(i = 0; i <= player[0].index; i++)
	{
		x = player[0].Record[i][0];
		y = player[0].Record[i][1];
		Right_X = CHESSBOARD_START_X + y * LENTH_OF_SQR + LENTH_OF_SQR/2;
		Center_Y = CHESSBOARD_START_Y - x * LENTH_OF_SQR;
		MovePen(Right_X, Center_Y);
		StartFilledRegion(1);
		SetPenColor("BLACK");
		DrawArc(LENTH_OF_SQR/2, 0, 360);
		EndFilledRegion();
	}
	for(i = 0; i <= player[1].index; i++)
	{
		x = player[1].Record[i][0];
		y = player[1].Record[i][1];
		Right_X = CHESSBOARD_START_X + y * LENTH_OF_SQR + LENTH_OF_SQR/2;
		Center_Y = CHESSBOARD_START_Y - x * LENTH_OF_SQR;
		MovePen(Right_X, Center_Y);
		StartFilledRegion(1);
		SetPenColor("RICE");
		DrawArc(LENTH_OF_SQR/2, 0, 360);
		EndFilledRegion();
	}
}

int JudgeXY(int x, int y, int *location)
{
	int i, j;
	double k;
	int flag = NONE;
	double myx = ScaleXInches(x), myy = ScaleYInches(y);

	double current_x, current_y;
	if(myx >= TAKEBACK_X && myx <= TAKEBACK_X2 && myy >= TAKEBACK_Y && myy <= TAKEBACK_Y2)
		return TAKEBACK;

	for(i = 0; i < MAPSIZE; i++)
	{
		for(j = 0; j < MAPSIZE; j++)
		{
			current_x = CHESSBOARD_START_X + j * LENTH_OF_SQR;
			current_y = CHESSBOARD_START_Y - i * LENTH_OF_SQR;
			k = (current_x-myx)*(current_x-myx) + (current_y-myy)*(current_y-myy);
			
			if(k <= LENTH_OF_SQR*LENTH_OF_SQR/4)
			{
				if(map[i][j] != -1)
					return NONE;
				location[0] = i;
				location[1] = j;
				return PLAYCHESS;
			}
		}
	}
	return NONE;
}

int CheckStartChoice(int x, int y)
{
	double myx, myy;
	
	myx = ScaleXInches(x);
	myy = ScaleYInches(y);
	
	if(myx >= STARTSCREEN_PLAY_X1 && myx <= STARTSCREEN_PLAY_X2 && myy >= STARTSCREEN_PLAY_Y1 && myy <= STARTSCREEN_PLAY_Y2)
		return PLAY;
	if(myx >= STARTSCREEN_ONLINE_X1 && myx <= STARTSCREEN_ONLINE_X2 && myy >= STARTSCREEN_ONLINE_Y1 && myy <= STARTSCREEN_ONLINE_Y2)
		return ONLINE;
	if(myx >= STARTSCREEN_EXIT_X1 && myx <= STARTSCREEN_EXIT_X2 && myy >= STARTSCREEN_EXIT_Y1 && myy <= STARTSCREEN_EXIT_Y2)
		return EXIT;
	else
		return NONE;
}

int CheckWinChoice(int x, int y)
{
	double myx = ScaleXInches(x), myy = ScaleYInches(y);

	if(myx >= RESTART_X && myx <= RESTART_X2 && myy >= RESTART_Y && myy <= RESTART_Y2 && IsOnline == FALSE)
		return RESTART;
	if(myx >= WIN_EXIT_X && myx <= WIN_EXIT_X2 && myy >= WIN_EXIT_Y && myy <= WIN_EXIT_Y2)
		return EXIT;
	return NONE;
}
int CheckOnlineChoice(int x, int y)
{
	double myx = ScaleXInches(x), myy = ScaleYInches(y);
	if(myx >= ONLINE_SCREEN_SERVER_X1 && myx <= ONLINE_SCREEN_SERVER_X2 && myy >= ONLINE_SCREEN_SERVER_Y1 && myy <= ONLINE_SCREEN_SERVER_Y2)
		return SERVER;
	if(myx >= ONLINE_SCREEN_CLIENT_X1 && myx <= ONLINE_SCREEN_CLIENT_X2 && myy >= ONLINE_SCREEN_CLIENT_Y1 && myy <= ONLINE_SCREEN_CLIENT_Y2)
		return CLIENT;
	else 
		return NONE;
}


void DrawRec(double x, double y)
{
	DrawLine(x, 0);
	DrawLine(0, -y);
	DrawLine(-x, 0);
	DrawLine(0, y);
}
void CleanChessBoard(double Right_X, double Center_Y)
{
	SetEraseMode(TRUE);
	MovePen(Right_X, Center_Y);
	DrawLine(-LENTH_OF_SQR, 0);
	MovePen(Right_X-LENTH_OF_SQR/2, Center_Y-LENTH_OF_SQR/2);
	DrawLine(0, LENTH_OF_SQR);
	SetEraseMode(FALSE);
	
}
int ChangePlayer(int Current_player)
{
	if(Current_player == BLACK)
		return WHITE;
	else
		return BLACK;
}

void ChangeInformation(int x, int y)
{

	PlayChoose = JudgeXY(x, y, location);
	printf("your_PlayChoose = %d\n", PlayChoose);
	if(PlayChoose == PLAYCHESS)
	{
		player[Your_Player].Record[Current_index][0] = location[0];
		player[Your_Player].Record[Current_index][1] = location[1];
		player[Your_Player].index = Current_index;
		Judge(map, &player[Your_Player]);
		CanDrop = TRUE;
		DrawPiece(player[Your_Player]);
		repaint();
		/*SetEraseMode(TRUE);
		MovePen(0, 7);
		StartFilledRegion(1);
		DrawRec(10, 7);
		EndFilledRegion();
		SetEraseMode(FALSE);
		DrawChessBoard();
		ReDrawMap(player);*/
	}
	else if(PlayChoose == TAKEBACK)
	{
		if(Current_index == 0 && Your_Player == BLACK)
			return;

		if ((IsOnline == TRUE && CanDrop == FALSE) || IsOnline == FALSE)
		{
			SetEraseMode(TRUE);
			MovePen(0, 7);
			StartFilledRegion(1);
			DrawRec(10, 7);
			EndFilledRegion();
			SetEraseMode(FALSE);
			DrawChessBoard();
			ReDrawMap(player);
		}
	}
	if(player[Your_Player].IsWinner == TRUE)
	{
		printf("Win\n");
		SetEraseMode(TRUE);
		MovePen(0, 7);
		StartFilledRegion(1);
		DrawRec(10, 7);
		EndFilledRegion();
		SetEraseMode(FALSE);
		printf("clean\n");
		Winner = Your_Player;
		DrawWinScreen(player[Winner]);
		Mode = WIN;
		printf("Mode Changed!\n");
	}
	else
	{
		if(Current_player == WHITE)
			Current_index++;
	} 

}
void parse_string(char *str,int *location)
{
    int index = 1;
    int num=0;
    while(str[index] != '\0')
    {
        if (str[index] != ',')
        {
            num = str[index] - '0' + num * 10;
        }
        else
        {
            location[0] = num;
            num = 0;
        }
        index++;
    }
    location[1] = num;
}
