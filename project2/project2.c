#include <stdio.h>
void parse_string(char *str, int * location);
void Judge(int (*map)[MAP_SIZE], PLAYER *player);
int main()
{
    return 0;
}
/*
int main(void)
{
    char* test = "12,11";
    int location[2];
    parse_string(test, location);
    printf("%d, %d",location[0],location[1]);
    return 0;
}
//test
*/
void parse_string(char *str,int *location)
{
    int index = 0;
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
void Judge(int (*map)[MAP_SIZE], PLAYER * player)
{
    int x,y,count = 0;
    int i,j;
    x = player->Record[index][0];
    y = player->Record[index][1];
    map[x][y] = player->color;
    for (i = 0;i < 2;i++)
        for (j = 0;j < 2;j++)
        {
            if (i == 0 && j == 0)
                continue;
            while (map[x][y] == player->color && x>=0 && x < MAP_SIZE
            && y >= 0 && y < MAP_SIZE)
            {
                x += i;
                y += j;
                count++;
            }
            x = player->Record[index][0];
            y = player->Record[index][1];
            while (map[x][y] == player->color && x>=0 && x < MAP_SIZE
            && y >= 0 && y < MAP_SIZE)
            {
                x -= i;
                y -= j;
                count++;
            }
            if (count - 1 >= 5)
            {
                player->IsWinner = 1;
                return;
            }
            count = 0;
        }
    
    i = 1;
    j = -1;
    //the remain condition

    while (map[x][y] == player->color && x >= 0 && x < MAP_SIZE
    && y >= 0 && y < MAP_SIZE)
    {
        x += i;
        y += j;
        count++;
    }
    x = player->Record[index][0];
    y = player->Record[index][1];
    while (map[x][y] == player->color && x >= 0 && x < MAP_SIZE
    && y >= 0 && y < MAP_SIZE)
    {
        x -= i;
        y -= j;
        count++;
    }
    if (count - 1 >= 5)
    {
        player->IsWinner = 1;
        return;
    }
}

