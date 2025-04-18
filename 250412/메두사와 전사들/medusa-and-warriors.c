#include <stdio.h>
#include <stdbool.h>
typedef struct _pos{
    int y;
    int x;
} pos;
typedef struct _jeonsa{
    pos p;
    bool is_active;
    bool is_alive;
} jeonsa;
typedef struct _queue{
    pos arr[2500];
    int start;
    int len;
} queue;
void push(queue* q, pos p)
{
    q->arr[q->start+q->len++] = p;
}
pos pop(queue* q)
{
    q->len--;
    return q->arr[q->start++];
}
typedef struct _mapelement{
    int wall; // 0: road, 1: wall, 2: swamp(wall은 처음 BFS 이후 그렇게 중요한 정보가 아님)
    int jeonsano;
    int jeonsa_array_in_mapelement[301];
} mapelement;
int abs(int x)
{
    return (x>0)?x:(-x);
}
int dist(pos p, pos dest)
{
    return abs(p.y-dest.y) + abs(p.x-dest.x);
}
bool is_equal_pos(pos p1, pos p2)
{
    if(p1.y == p2.y && p1.x == p2.x) return true;
    else return false;
}
jeonsa jeonsaarray[301];
pos route[50][50];
pos prev[50][50];
bool check[50][50];
bool wax[50][50];
mapelement map[50][50];
mapelement newmap[50][50];
int N, M;
int dy[4] = {-1, 0, 1, 0};
int dx[4] = {0, 1, 0, -1};
int ddy[4] = {-1, 1, 1, -1};
int ddx[4] = {1, 1, -1, -1};
int priority1[4] = {0, 2, 3, 1};
int priority2[4] = {3, 1, 0, 2};
bool bound(int y, int x)
{
    if(y == -1 || y == N || x == -1 || x == N) return true;
    else return false;
}
int BFS_medusa(pos p, pos dest)
{
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            check[i][j] = false;
            prev[i][j].y = -1;
        }
    }
    queue q;
    q.start = 0;
    q.len = 0;
    push(&q, p);
    check[p.y][p.x] = true;
    while(q.len)
    {
        pos pop_p = pop(&q);
        if(is_equal_pos(pop_p, dest)) break;
        for(int dir = 0;dir<=3;dir++)
        {
            pos new_p;
            new_p.y = pop_p.y+dy[priority1[dir]];
            new_p.x = pop_p.x+dx[priority1[dir]];
            if(!bound(new_p.y, new_p.x) && !check[new_p.y][new_p.x] && !map[new_p.y][new_p.x].wall) // medusa는 wall을, jeonsa는 swamp를 피함
            {
                check[new_p.y][new_p.x] = true;
                push(&q, new_p);
                prev[new_p.y][new_p.x] = pop_p;
            }
        }
    }
    if(prev[dest.y][dest.x].y == -1) return -1;
    pos temp_p = dest;
    pos temptemp_p;
    while(!is_equal_pos(temp_p, p))
    {
        temptemp_p = prev[temp_p.y][temp_p.x];
        route[temptemp_p.y][temptemp_p.x] = temp_p;
        temp_p = temptemp_p;
    }
    return 0;
}

void BFS_medusa_search_jeonsa(pos p, int di_dir, int st_dir)
{
    queue q;
    q.start = 0;
    q.len = 0;
    push(&q, p);
    check[p.y][p.x] = true;
    wax[p.y][p.x] = true;
    while(q.len)
    {
        pos pop_p = pop(&q);
        pos di_p, st_p;
        di_p.y = pop_p.y+ddy[di_dir];
        di_p.x = pop_p.x+ddx[di_dir];
        st_p.y = pop_p.y+dy[st_dir];
        st_p.x = pop_p.x+dx[st_dir];
        if(!bound(di_p.y, di_p.x) && !check[di_p.y][di_p.x])
        {
            check[di_p.y][di_p.x] = true;
            wax[di_p.y][di_p.x] = true;
            push(&q, di_p);
        }
        if(!bound(st_p.y, st_p.x) && !check[st_p.y][st_p.x])
        {
            check[st_p.y][st_p.x] = true;
            wax[st_p.y][st_p.x] = true;
            push(&q, st_p);
        }
    }
}

int BFS_medusa_search(pos p, int di_dir, int st_dir, int um)
{
    // um이 0일 때 확인
    // um이 1일 때 색칠
    int count = 0;
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            check[i][j] = false; // check 배열 재활용
            wax[i][j] = false;
        }
    }
    if(bound(p.y, p.x)) return 0;
    if(um) map[p.y][p.x].wall = 2;
    if(map[p.y][p.x].jeonsano)
    {
        if(um)
            for(int i=0;i<map[p.y][p.x].jeonsano;i++)
                jeonsaarray[map[p.y][p.x].jeonsa_array_in_mapelement[i]].is_active = false;
        return map[p.y][p.x].jeonsano;
    }
    queue q;
    q.start = 0;
    q.len = 0;
    push(&q, p);
    check[p.y][p.x] = true;
    while(q.len)
    {
        pos pop_p = pop(&q);
        pos di_p, st_p;
        di_p.y = pop_p.y+ddy[di_dir];
        di_p.x = pop_p.x+ddx[di_dir];
        st_p.y = pop_p.y+dy[st_dir];
        st_p.x = pop_p.x+dx[st_dir];
        if(!bound(di_p.y, di_p.x) && !check[di_p.y][di_p.x] && !wax[di_p.y][di_p.x])
        {
            check[di_p.y][di_p.x] = true;
            if(um) map[di_p.y][di_p.x].wall = 2;
            if(map[di_p.y][di_p.x].jeonsano) // 전사가 위치한 칸, 다른 칸들은 보이지 않게
            {
                if(um)
                    for(int i=0;i<map[di_p.y][di_p.x].jeonsano;i++)
                        jeonsaarray[map[di_p.y][di_p.x].jeonsa_array_in_mapelement[i]].is_active = false;
                else count += map[di_p.y][di_p.x].jeonsano;
                BFS_medusa_search_jeonsa(di_p, di_dir, st_dir);
            }
            else push(&q, di_p);
        }
        if(!bound(st_p.y, st_p.x) && !check[st_p.y][st_p.x] && !wax[st_p.y][st_p.x])
        {
            check[st_p.y][st_p.x] = true;
            if(um) map[st_p.y][st_p.x].wall = 2;
            if(map[st_p.y][st_p.x].jeonsano) // 전사가 위치한 칸, 다른 칸들은 보이지 않게
            {
                if(um)
                    for(int i=0;i<map[st_p.y][st_p.x].jeonsano;i++)
                        jeonsaarray[map[st_p.y][st_p.x].jeonsa_array_in_mapelement[i]].is_active = false;
                else count += map[st_p.y][st_p.x].jeonsano;
                BFS_medusa_search_jeonsa(st_p, di_dir, st_dir);
            }
            else push(&q, st_p);
        }
    }
    return count;
}
int main(void)
{
    scanf("%d %d", &N, &M);
    pos medusa_start_p, medusa_end_p;
    scanf("%d %d %d %d", &medusa_start_p.y, &medusa_start_p.x, &medusa_end_p.y, &medusa_end_p.x);
    for(int i=0;i<M;i++)
    {
        scanf("%d %d", &jeonsaarray[i].p.y, &jeonsaarray[i].p.x);
        map[jeonsaarray[i].p.y][jeonsaarray[i].p.x].jeonsa_array_in_mapelement[map[jeonsaarray[i].p.y][jeonsaarray[i].p.x].jeonsano++] = i;
        jeonsaarray[i].is_active = true;
        jeonsaarray[i].is_alive = true;
    }
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            scanf("%d", &map[i][j].wall);
    if (BFS_medusa(medusa_start_p, medusa_end_p) == -1)
    {
        printf("-1");
        return 0;
    }
    pos medusa_p = medusa_start_p;
    int time = 0;
    while(1)
    {
        for(int i=0;i<N;i++)
        {
            for(int j=0;j<N;j++)
            {
                map[i][j].wall = 0;
                newmap[i][j].jeonsano = 0;
            }
                
        }
        // 메두사 이동
        medusa_p = route[medusa_p.y][medusa_p.x];
        if(map[medusa_p.y][medusa_p.x].jeonsano)
            for(int i=0;i<map[medusa_p.y][medusa_p.x].jeonsano;i++)
                jeonsaarray[map[medusa_p.y][medusa_p.x].jeonsa_array_in_mapelement[i]].is_alive = false;
        map[medusa_p.y][medusa_p.x].jeonsano = 0;
        
        if(is_equal_pos(medusa_p, medusa_end_p)) break;

        // 메두사 시선
        // 대각선, 수직, 수평 탐색
        // 대각선도 BFS 돌리죠 ㅇㅇ
        int st[4] = {0, 0, 0, 0}; // 행열 개수 세기
        for(int dir = 0;dir<=3;dir++)
        {
            pos st_p;
            st_p.y = medusa_p.y;
            st_p.x = medusa_p.x;
            while(1)
            {
                st_p.y += dy[dir];
                st_p.x += dx[dir];
                if(bound(st_p.y, st_p.x)) break;
                if(map[st_p.y][st_p.x].jeonsano)
                {
                    st[dir] = map[st_p.y][st_p.x].jeonsano;
                    break;
                }
            }
        }
        // 그 사이 탐색
        int sai[8] = {0};
        int startpos_y[8] = {-1, -1, 1, 1, 1, 1, -1, -1};
        int startpos_x[8] = {1, 1, 1, 1, -1, -1, -1, -1};
        for(int dir = 0;dir<=7;dir++)
        {
            pos start_p;
            start_p.y = medusa_p.y+startpos_y[dir];
            start_p.x = medusa_p.x+startpos_x[dir];
            sai[dir] = BFS_medusa_search(start_p, dir/2, ((dir+1)/2)%4, 0);
        }
        int dirsum[4];
        for(int dir=0;dir<=3;dir++)
        {
            dirsum[dir] = st[dir] + sai[dir*2] + sai[(dir*2+7)%8];
        }
        int maxdir = -1;
        int maxinactivate = -1;
        for(int dir = 0;dir<=3;dir++)
        {
            if(dirsum[priority1[dir]] > maxinactivate)
            {
                maxinactivate = dirsum[priority1[dir]];
                maxdir = priority1[dir];
            }
        }

        // map에 색칠놀이
        pos st_p;
        st_p.y = medusa_p.y;
        st_p.x = medusa_p.x;
        while(1)
        {
            st_p.y += dy[maxdir];
            st_p.x += dx[maxdir];
            if(bound(st_p.y, st_p.x)) break;
            map[st_p.y][st_p.x].wall = 2; // 색칠놀이
            if(map[st_p.y][st_p.x].jeonsano)
            {
                for(int i=0;i<map[st_p.y][st_p.x].jeonsano;i++)
                    jeonsaarray[map[st_p.y][st_p.x].jeonsa_array_in_mapelement[i]].is_active = false;
                break;
            }
        }
        pos start_p;
        start_p.y = medusa_p.y+startpos_y[maxdir*2];
        start_p.x = medusa_p.x+startpos_x[maxdir*2];
        BFS_medusa_search(start_p, maxdir, maxdir, 1);
        start_p.y = medusa_p.y+startpos_y[(maxdir*2+7)%8];
        start_p.x = medusa_p.x+startpos_x[(maxdir*2+7)%8];
        BFS_medusa_search(start_p, (maxdir+3)%4, maxdir, 1);
        // 전사 이동
        int diedjeonsa = 0;
        int distjeonsa = 0;
        for(int i=0;i<M;i++)
        {
            if(jeonsaarray[i].is_active && jeonsaarray[i].is_alive)
            {
                pos og_p = jeonsaarray[i].p;
                pos new_p;
                // 전사 1차 이동
                bool moved = false;
                for(int dir = 0;dir<=3;dir++)
                {
                    new_p.y = og_p.y + dy[priority1[dir]];
                    new_p.x = og_p.x + dx[priority1[dir]];
                    if(!bound(new_p.y, new_p.x) && !map[new_p.y][new_p.x].wall && dist(og_p, medusa_p) > dist(new_p, medusa_p))
                    {
                        jeonsaarray[i].p = new_p;
                        distjeonsa++;
                        if(is_equal_pos(new_p, medusa_p))
                        {
                            jeonsaarray[i].is_alive = false;
                            diedjeonsa++;
                            goto um;
                        }
                        moved = true;
                        break;
                    }
                }
                if(!moved)
                {
                    newmap[og_p.y][og_p.x].jeonsa_array_in_mapelement[newmap[og_p.y][og_p.x].jeonsano++] = i;
                    continue;
                }
                // 전사 2차 이동
                og_p = jeonsaarray[i].p;
                moved = false;
                for(int dir = 0;dir<=3;dir++)
                {
                    new_p.y = og_p.y + dy[priority2[dir]];
                    new_p.x = og_p.x + dx[priority2[dir]];
                    if(!bound(new_p.y, new_p.x) && !map[new_p.y][new_p.x].wall && dist(og_p, medusa_p) > dist(new_p, medusa_p))
                    {
                        jeonsaarray[i].p = new_p;
                        distjeonsa++;
                        if(is_equal_pos(new_p, medusa_p))
                        {
                            jeonsaarray[i].is_alive = false;
                            diedjeonsa++;
                            goto um;
                        }
                        newmap[new_p.y][new_p.x].jeonsa_array_in_mapelement[newmap[new_p.y][new_p.x].jeonsano++] = i;
                        moved = true;
                        break;
                    }
                }
                if(!moved)
                {
                    newmap[og_p.y][og_p.x].jeonsa_array_in_mapelement[newmap[og_p.y][og_p.x].jeonsano++] = i;
                    continue;
                }
            }
            um:
            continue;
        }
        // 석화된 전사 풀림, newmap copy
        for(int i=0;i<N;i++)
        {
            for(int j=0;j<N;j++)
            {
                for(int k=0;k<newmap[i][j].jeonsano;k++)
                    map[i][j].jeonsa_array_in_mapelement[k] = newmap[i][j].jeonsa_array_in_mapelement[k];
                map[i][j].jeonsano = newmap[i][j].jeonsano;
            }
        }
        for(int i=0;i<M;i++)
        {
            if(jeonsaarray[i].is_alive && !jeonsaarray[i].is_active)
            {
                jeonsaarray[i].is_active = true;
                pos og_p = jeonsaarray[i].p;
                map[og_p.y][og_p.x].jeonsa_array_in_mapelement[map[og_p.y][og_p.x].jeonsano++] = i;
            }
        }
        printf("%d %d %d\n", distjeonsa, maxinactivate, diedjeonsa);
        time++;
    }
    printf("0");
    return 0;
}