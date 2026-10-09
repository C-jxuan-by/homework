#ifndef GAMEFUNCTIONS_H
#define GAMEFUNCTIONS_H

#include <gameData.h>
#include <cstdlib>
#include <ctime>

int set_bit(int num, int pos)
{
    return num | (1 << pos);
}
int get_thiscub_data(int y,int x)
{
    int yy=cub.state[cub.dir][y];
    return yy&code[x];
}
int get_mapcub_data(int y,int x)
{
    int yy=mapp[y];
    return yy&code[x];
}
void putcub(char x, struct russiacubs* cub)
{
    switch (x)
    {
    case 'I':
        *cub = I;
        break;
    case 'T':
        *cub = T;
        break;
    case 'J':
        *cub = J;
        break;
    case 'L':
        *cub = L;
        break;
    case 'O':
        *cub = O;
        break;
    case 'S':
        *cub = S;
        break;
    case 'Z':
        *cub = Z;
        break;
    default:
        break;
    }
}
void rotate_cub()
{
    int c=(cub.dir+1)%4;
    cub.dir = c;
    int x=cub.position;
    if (x < 0)
        cub.position = 0;
    else if(x > X - cub.wide[c])
        cub.position = X - cub.wide[c];
}
int leftMove_cub()
{
    int d = cub.dir;
    int x=cub.position-1;
    if (x >= 0 && x <= X - cub.wide[d])
    {
        cub.position = x;
        return 1;
    }
    else if (x < 0)
        cub.position = 0;
    else
        cub.position = X - cub.wide[d];
    return 0;
}
int rightMove_cub()
{
    int d = cub.dir;
    int x=cub.position+1;
    if (x >= 0 && x <= X - cub.wide[d])
    {
        cub.position = x;
        return 1;
    }
    else if (x < 0)
        cub.position = 0;
    else
        cub.position = X - cub.wide[d];
    return 0;
}
void random_cub(struct russiacubs* cub)
{
    srand(static_cast<unsigned int>(time(0)));
    int x=rand()%7;
    putcub(cubtype[x],cub);
}

void set_cub()
{
    int x = cub.position;
    int d = cub.dir;
    //放置在死线上
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < cub.wide[d]; j++)
        {
            if (cub.state[d][i] & code[j])
                mapp[DIE + i] = set_bit(mapp[DIE + i], x + j);
        }
    }

    //找出下落距离
    int nearest = Y + 1;
    for (int i = 0; i < cub.wide[d]; i++)
    {
        int y = DIE;
        while (!(mapp[y]&code[x+i]))
            y++;
        int dis = 0;
        while (!(mapp[y - dis - 1]&code[x+i]))
        {
            dis++;
            if (y - dis - 1 < 0)
                break;
        }
        if (nearest > dis)	nearest = dis;
    }
    //printf("%d\n", nearest);
    // 清空
    for (int i = 0; i < 4; i++)
    {
        mapp[DIE + i] = 0;
    }

    //放置
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < cub.wide[d]; j++)
        {
            if (cub.state[d][i] & code[j])
                mapp[DIE + i-nearest] |= set_bit(mapp[DIE + i], x + j);
        }
    }
}

int clear_ful()
{
    int lines = 0;
    //int ful = 1;
    for (int i = 0; i < Y - 1; i++)
    {
        if (mapp[i]+1==1024)
        {
            lines++;
            for (int y = i; y < Y - 1; y++)
            {
                mapp[y] = mapp[y + 1];
            }
            i--;
        }
    }
    return score[lines];
}


#endif // GAMEFUNCTIONS_H
