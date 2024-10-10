#include <stdio.h>
#include <stdlib.h>
typedef struct _coordinate{
    int ypos;
    int xpos;
} coordinate;
int map[7][7];
int tempmap[7][7];
int checked[7][7];
int dy[4] = {0, 1, 0, -1};
int dx[4] = {1, 0, -1, 0};
int K, M;
int cnt, sum, max;
int* byeok;
coordinate temptemparray[25];
coordinate temparray[25];
coordinate array[25];
void dfs(int ypos, int xpos)
{
    coordinate current_pos = {ypos, xpos};
    temptemparray[cnt++] = current_pos;
    checked[ypos][xpos] = 1;
    for(int dir = 0;dir<=3;dir++)
    {
        if(tempmap[ypos+dy[dir]][xpos+dx[dir]] == tempmap[ypos][xpos] && !checked[ypos+dy[dir]][xpos+dx[dir]])
        {
            dfs(ypos+dy[dir], xpos+dx[dir]);
        }
    }
}
void find(void)
{
    for(int i=1;i<=5;i++)
    {
        for(int j=1;j<=5;j++)
        {
            if(!checked[i][j])
            {
                cnt = 0;
                // temptemparray 초기화
                for(int initialize_temptemparray = 0;initialize_temptemparray<=24;initialize_temptemparray++)
                {
                    coordinate initial = {-1, -1};
                    temptemparray[initialize_temptemparray] = initial;
                }
                dfs(i, j);
                if(cnt >= 3)
                {
                    // temparray 뒤에 temptemparray 붙이기
                    for(int moving_temptemparray = 0;moving_temptemparray <= cnt-1;moving_temptemparray++)
                    {
                        temparray[sum + moving_temptemparray] = temptemparray[moving_temptemparray];
                    }
                    sum = sum + cnt;
                }
            }
        }
    }
}
int main(void)
{
    int byeokpointer = 0;
    scanf("%d %d", &K, &M);
    byeok = (int*)malloc(sizeof(int) * M);
    for(int i=0;i<=6;i++)
    {
        map[i][0] = 0;
        map[0][i] = 0;
        map[i][6] = 0;
        map[6][i] = 0;
    }
    for(int i=1;i<=5;i++)
    {
        for(int j=1;j<=5;j++)
        {
            scanf("%d", &map[i][j]);
        }
    }
    for(int i=0;i<=M-1;i++)
    {
        scanf("%d", &byeok[i]);
    }
    for(int tamsa = 0;tamsa<=K-1;tamsa++)
    {
        int total = 0;
        // 탐사 진행
        int max = 0;
        int maxangle, maxj, maxi;
        for(int angle = 270;angle >= 90;angle -= 90)
        {
            for(int j=4;j>=2;j--)
            {
                for(int i=4;i>=2;i--)
                {
                    for(int new_i=0;new_i<=6;new_i++)
                    {
                        for(int new_j=0;new_j<=6;new_j++)
                        {
                            tempmap[new_i][new_j] = map[new_i][new_j];
                            checked[new_i][new_j] = 0;
                        }
                    }
                    sum = 0;
                    // temparray 초기화
                    for(int initialize_temparray = 0;initialize_temparray<=24;initialize_temparray++)
                    {
                        coordinate initial = {-1, -1};
                        temparray[initialize_temparray] = initial;
                    }
                    // 회전
                    for(int spin = 1;spin<=angle/90;spin++)
                    {
                        int temp = tempmap[i-1][j-1];
                        tempmap[i-1][j-1] = tempmap[i+1][j-1];
                        tempmap[i+1][j-1] = tempmap[i+1][j+1];
                        tempmap[i+1][j+1] = tempmap[i-1][j+1];
                        tempmap[i-1][j+1] = temp;
                        temp = tempmap[i][j-1];
                        tempmap[i][j-1] = tempmap[i+1][j];
                        tempmap[i+1][j] = tempmap[i][j+1];
                        tempmap[i][j+1] = tempmap[i-1][j];
                        tempmap[i-1][j] = temp;
                    }
                    // 찾기
                    find();
                    if(sum >= max)
                    {
                        max = sum;
                        maxangle = angle;
                        maxi = i;
                        maxj = j;
                        // array에 temparray copy
                        for(int copying_temparray = 0;copying_temparray<=sum-1;copying_temparray++)
                        {
                            array[copying_temparray] = temparray[copying_temparray];
                        }
                    }
                }
            }
        }
        if(max == 0)
            break;
        total += max;
        for(int spin = 1;spin<=maxangle/90;spin++)
        {
            int temp = map[maxi-1][maxj-1];
            map[maxi-1][maxj-1] = map[maxi+1][maxj-1];
            map[maxi+1][maxj-1] = map[maxi+1][maxj+1];
            map[maxi+1][maxj+1] = map[maxi-1][maxj+1];
            map[maxi-1][maxj+1] = temp;
            temp = map[maxi][maxj-1];
            map[maxi][maxj-1] = map[maxi+1][maxj];
            map[maxi+1][maxj] = map[maxi][maxj+1];
            map[maxi][maxj+1] = map[maxi-1][maxj];
            map[maxi-1][maxj] = temp;
        }
        // 유물 획득
        while(1)
        {
            // array 정렬
            for(int i=0;i<=max-2;i++)
            {
                for(int j=i+1;j<=max-1;j++)
                {
                    if(array[i].xpos > array[j].xpos || (array[i].xpos == array[j].xpos && array[i].ypos < array[j].ypos))
                    {
                        coordinate temp = {array[i].ypos, array[i].xpos};
                        array[i].ypos = array[j].ypos;
                        array[i].xpos = array[j].xpos;
                        array[j].ypos = temp.ypos;
                        array[j].xpos = temp.xpos;
                    }
                }
            }
            // map 변경
            for(int i=0;i<=max-1;i++)
            {
                map[array[i].ypos][array[i].xpos] = byeok[byeokpointer+i];
            }
            byeokpointer += max;
            // checked 초기화
            for(int i=0;i<=6;i++)
            {
                for(int j=0;j<=6;j++)
                {
                    checked[i][j] = 0;
                    tempmap[i][j] = map[i][j];
                }
            }
            sum = 0;
            find();
            max = sum;
            if(max == 0)
                break;
            total += max;
            for(int copying_temparray = 0;copying_temparray<=max-1;copying_temparray++)
            {
                array[copying_temparray] = temparray[copying_temparray];
            }
        }
        printf("%d ", total);
    }
    free(byeok);
    return 0;
}