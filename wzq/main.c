#define NONE -1
#define START 0
#define INTRODUCTION 1
#define PLAY 2
#define EXIT 3
#define WIN 3
#define MAPSIZE 19
#define PLAYCHESS 1
#define TAKEBACK 2
#define BLACK 0
#define WHITE 1
#define RESTART 1;

#define HeadColor_Red 		145
#define HeadColor_Green		85
#define HeadColor_Blue		57
#define PLAY_X 				4
#define PLAY_Y  			4
#define EXIT_X              4
#define EXIT_Y              2
#define TITLE_X             4
#define TITLE_Y             5
#define PENSIZE_OF_HEAD     5
#define CHESSBOARD_START_X  1
#define CHESSBOARD_START_Y  6
#define LENTH_OF_SQR        0.25
#define PLAYER_PEN_SIZE     5
#define PLAYER1_X			8
#define PLAYER1_Y			5
#define PLAYER2_X  			8
#define PLAYER2_Y			3
#define TAKEBACK_PEN_SIZE	4
#define TAKEBACK_X			8
#define TAKEBACK_Y			4
#define YOUWIN_PEN_SIZE		10
#define YOUWIN_X			4
#define YOUWIN_Y			4
#define WINSTEPS_PEN_SIZE	15
#define WINSTEPS_X			2
#define WINSTEPS_Y			3
#define RESTART_PEN_SIZE	4
#define RESTART_X			4
#define RESTART_Y			2
#define WIN_EXIT_X			4
#define WIN_EXIT_Y			1
#define STARTSCREEN_PLAY_X1 4
#define STARTSCREEN_PLAY_X2 4+TextStringWidth("kaishiyouxi")
#define STARTSCREEN_PLAY_Y1 4
#define STARTSCREEN_PLAY_Y2 4.5
#define STARTSCREEN_EXIT_X1 4
#define STARTSCREEN_EXIT_X2 4+TextStringWidth("tuichu")
#define STARTSCREEN_EXIT_Y1 2
#define STARTSCREEN_EXIT_Y2 2.5

typedef struct
{
	int color;
	int Record[180][2];
	int IsWinner;
	int index;
}PLAYER;

void MouseEventProcess(int x, int y, int button, int event);

void DrawStartScreen();
void DrawChessBoard();
void DrawWinScreen(PLAYER Winner);
void DrawPiece(PLAYER piece);
void ReDrawMap(PLAYER player[]);
void Judge(int map[][], PLAYER* player);

int JudgeXY(int x, int y, location[]);
int CheckStartChoice(int x, int y);			
int ChangePlayer(int Current_player);

void Main()
{
	DrawStartScreen();
	registerMouseEvent(MouseEventProcess);
}

void MouseEventProcess(int x, int y, int button, int event)
{
	int i, j;
	static int Winner = -1;
	static int PlayChoose;
	static int WinChoice;
	static int Current_index = 0;
	static int Current_player = BLACK;
	static int location[2];
	static int map[MAPSIZE][MAPSIZE] = {0,0};
	static PLAYER player[2] = 	{
								{BLACK, {0,0}, FALSE, 0},
								{WHITE, {0,0}, FALSE, 0}
								};
	static int Mode = START;
	static int StartChoice;
	switch(Mode)
	{
		case START:
		{
			if(event == BUTTON_DOWN)
			{
				StartChoice = CheckStartChoice(x, y);
				switch(choice)
				{
					case NONE:
						break;
					case PLAY:
						Mode = PLAY;
						SetEraserMode(TRUE);
						DrawStartScreen();
						SetEraserMode(FALSE);
						DrawChessBoard();
						break;
					case EXIT:
						ExitGraphics();
						break;
				}
			}
			break;
		}
		case INTRODUCTION:
		{
			if(event == BUTTON_DOWN)
			{
				SetEraserMode(TRUE);
				DrawIntroduction();
				SetEraserMode(FALSE);
				Mode = START;
				DrawStartScreen();
			}
			break;
		}
		case PLAY:
		{
			if(button == LEFT_BUTTON && event == BUTTON_DOWN)
			{
				PlayChoose = JudgeXY(x, y, location);
				if(PlayChoose == PLAYCHESS)											//Change the information and redraw the picture.
				{
					player[Current_player].Record[Current_index][0] = location[0];
					player[Current_player].Record[Current_index][1] = location[1];
					player[Current_player].index = Current_index;
					Judge(map, PLAYER* player);
					DrawPiece(player[Current_player]);
				}
				else if(PlayChoose == TAKEBACK)
				{
					if(Current_index == 0)
						break;

					SetEraserMode(TRUE);
					DrawChessBoard();
					ReDrawMap(player);
					SetEraserMode(FALSE);
					if(Current_player == BLACK)
						Current_index--;
					Current_player = ChangePlayer(Current_player);
					player[Current_player].index = Current_index-1;
					DrawChessBoard();
					ReDrawMap(player);
				}
			}
			else if(button == LEFT_BUTTON && event == BUTTON_UP)					//判断胜利，改变玩家
			{
				if(PlayChoose == PLAYCHESS)
				{
					if(player[Current_player].IsWinner == TRUE)
					{
						SetEraserMode(TRUE);
						DrawChessBoard();
						ReDrawMap(player);
						SetEraserMode(FALSE);
						Winner = Current_player;
						DrawWinScreen(player[Winner]);
						Winner = Current_player;
						Mode = WIN;
					}
					if(Current_player == WHITE)
						Current_index++;
					Current_player = ChangePlayer(Current_player);
				}
			}
			break;
		}
		case WIN:
		{
			if(event == BUTTON_DOWN && button == LEFT_BUTTON)
			{
				WinChoice = CheckWincChoice(x, y);
				if(WinChoice == RESTART)
				{
					SetEraserMode(TRUE);
					DrawWinScreen(player[Winner]);
					SetEraserMode(FALSE);
					DrawChessBoard();
					Mode = PLAY;
					/*初始化*/
					Winner = -1;
					Current_index = 0;
					Current_player = BLACK;
					for(i = 0; i < MAPSIZE; i++)
						for(j = 0; j < MAPSIZE; j++)
							map[i][j] = 0;

					player[0] = {BLACK, {0,0}, FALSE, 0};
					player[1] = {WHITE, {0,0}, FALSE, 0};
				}
				else if(WinChoice == EXIT)
				{
					ExitGraphics();
				}
			}
		}
	}
}

void DrawStartScreen()
{
	DefineColor("HeadColor", HeadColor_Red, HeadColor_Green, HeadColor_Blue);
	MovePen(PLAY_X, PLAY_Y);
	DrawTextString("kaishiyouxi");
	MovePen(EXIT_X, EXIT_Y);
	DrawTextString("tuichu");
	MovePen(TITLE_X, TITLE_Y);
	if(GetEraseMode() == FALSE)
	{
		SetPenColor("HeadColor");
	}
	DrawTextString("wuziqi");
}

void DrawChessBoard()
{
	int i, j;
	
	for(i = 0; i < MAPSIZE-1; i++)
	{
		for(j = 0; j < MAPSIZE-1; j++)
		{
			Pause(1);
			MovePen(CHESSBOARD_START_X + j * LENTH_OF_SQR, CHESSBOARD_START_Y - i * LENTH_OF_SQR);
			DrawRec(LENTH_OF_SQR, LENTH_OF_SQR);
		}
	}

	MovePen(PLAYER1_X, PLAYER1_Y);
	DrawTextString("Player1 hei");
	MovePen(PLAYER2_X, PLAYER2_Y);
	DrawTextString("Player2 bai");
	MovePen(TAKEBACK_X, TAKEBACK_Y);
	DrawTextString("huiqi");
}

void DrawWinScreen(PLAYER Winner);
{
	int power = 10;
	int i;
	int steps = Winner.index+1;
	int temp;
	char Congratulation1[23] = "gongxini!player1!niyingle!";
	char Congratulation2[25] = "nizhiyongle";
	const int index1 = 13;
	int index2 = 8;
	if(Winner.color == WHITE)
		Congratulation1[index1] = '2';
	
	for(power = 10;steps/power != 0; power*=10)
	for(power/=10 ; power != 1; power/=10)
	{
		Congratulation2[index2] = steps % power + '0';
		index2++;
	}
	strcpy(&Congratulation2[index2], "jiujibaileduifang");
	MovePen(YOUWIN_X, YOUWIN_Y);
	DrawTextString(Congratulation1);
	MovePen(WINSTEPS_X, WINSTEPS_Y);
	DrawTextString(Congratulation2);
	MovePen(RESTART_X, RESTART_Y);
	DrawTextString("zailaiyiju");
	MovePen(WIN_EXIT_X, WIN_EXIT_Y);
	DrawTextString("tuichu");
}

void DrawPiece(PLAYER piece)
{
	int x = piece.Record[piece.index][0], y = piece.Record[piece.index][1];
	MovePen(CHESSBOARD_START_X + y * LENTH_OF_SQR + LENTH_OF_SQR/3, CHESSBOARD_START_Y - x * LENTH_OF_SQR);
	if(pirce.color == black)
		StartFilledRegion(1);
	DrawArc(LENTH_OF_SQR/3, 0, 360);
	if(pirce.color == black)
		EndFilledRegion();
}

void ReDrawMap(PLAYER player[])
{
	int i;
	int x, y;
	for(i = 0; i <= player[0].index; i++)
	{
		x = play[0].Record[i][0];
		y = play[0].Record[i][1];
		MovePen(CHESSBOARD_START_X + y * LENTH_OF_SQR + LENTH_OF_SQR/3, CHESSBOARD_START_Y - x * LENTH_OF_SQR);
		StartFilledRegion(1);
		DrawArc(LENTH_OF_SQR/3, 0, 360);
		EndFilledRegion();
	}
	for(i = 0; i <= play[1].index; i++)
	{
		x = play[1].Record[i][0];
		y = play[1].Record[i][1];
		MovePen(CHESSBOARD_START_X + y * LENTH_OF_SQR + LENTH_OF_SQR/3, CHESSBOARD_START_Y - x * LENTH_OF_SQR);
		DrawArc(LENTH_OF_SQR/3, 0, 360);
	}
}

int JudgeXY(int x, int y, int location[])
{
	int i, j;
	int k;
	int flag = NONE;
	double myx = ScaleXInches(x), myy = 7 - ScaleYInches(y);
	double current_x, current_y;
	if(myx >= TAKEBACK_X && myx <= TAKEBACK_X2 && myy <= TAKEBACK_Y && myy >= TAKEBACK_Y2)
		return TAKEBACK;
	for(i = 0; i < MAPSIZE; i++)
	{
		for(j = 0; j < MAPSIZE; j++)
		{
			current_x = CHESSBOARD_START_X + j * LENTH_OF_SQR;
			current_y = CHESSBOARD_START_Y - y * LENTH_OF_SQR;
			k = (current_x-myx)*(current_x-myx) + (current_y-myy)*(current_y-myy);
			if(k <= LENTH_OF_SQR*LENTH_OF_SQR/9)
			{
				flag = PLAYCHESS;
				location[0] = i;
				location[1] = j;
				return flag;
			}
		}
	}
}

int CheckStartChoice(int x, int y)
{
	double myx, myy;
	myx = ScaleXInches(x);
	myy = 7 - ScaleYInches(y);

	if(myx >= STARTSCREEN_PLAY_X1 && myx <= STARTSCREEN_PLAY_X2 && myy >= STARTSCREEN_PLAY_Y1 && myy <= STARTSCREEN_PLAY_Y2)
		return PLAY;
	if(myx >= STARTSCREEN_EXIT_X1 && myx <= STARTSCREEN_EXIT_X2 && myy >= STARTSCREEN_EXIT_Y1 && myy <= STARTSCREEN_EXIT_Y2)
		return EXIT;
	else
		return NONE;
}

int ChangePlayer(int Current_player)
{
	if(Current_player == BLACK)
		return WHITE;
	else
		return BLACK;
}