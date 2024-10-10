#include <stdio.h>
#include <stdlib.h>
#define MAX_DIST 200000
typedef struct _product{
    int id;
    int suik;
    int revenue;
    int destination;
} product;
typedef struct _maxheap{
    struct _product arr[30000];
    int pos[30000];
    int size;
} maxheap;
int** map;
int* dist;
int N, M;
void push(maxheap* hp, product element)
{
    hp->size++;
    int findingpos = hp->size;
    while(findingpos != 1 && element.suik > hp->arr[findingpos >> 1].suik)
    {
        hp->arr[findingpos] = hp->arr[findingpos >> 1];
        hp->pos[hp->arr[findingpos].id] = findingpos;
        findingpos = findingpos >> 1;
    }
    
    hp->arr[findingpos] = element;
    hp->pos[element.id] = findingpos;
}

product pop(maxheap* hp, int index)
{
    product r = hp->arr[index];
    hp->pos[r.id] = -1;
    product last = hp->arr[hp->size];
    hp->size--;
    int parent = index;
    int child = parent << 1;
    while(child <= hp->size)
    {
        if(child < hp->size && hp->arr[child].suik < hp->arr[child+1].suik)
            child++;
        if(hp->arr[child].suik < last.suik)
            break;
        hp->arr[parent] = hp->arr[child];
        hp->pos[hp->arr[parent].id] = parent;
        parent = child;
        child = child<<1;
    }
    hp->arr[parent] = last;
    hp->pos[last.id] = parent;
    return r;
}

void dijkstra(int source)
{
    dist[source] = 0;
    int* check = (int*)malloc(sizeof(int) * N);
    int size = N;
    for(int i=0;i<=N-1;i++)
    {
        if(i != source)
        {
            dist[i] = MAX_DIST;
        }
        check[i] = 0;
    }
    while(size >= 1)
    {
        int min = MAX_DIST;
        int mini;
        for(int i=0;i<=N-1;i++)
        {
            if(check[i] != 1 && dist[i] < min)
            {
                min = dist[i];
                mini = i;
            }
        }
        check[mini] = 1;
        for(int j=0;j<=N-1;j++)
        {
            if(check[j] != 1 && map[mini][j] != MAX_DIST && dist[mini]+map[mini][j] < dist[j])
            {
                dist[j] = dist[mini]+map[mini][j];
            }
        }
        size--;
    }
    free(check);
}
int main(void)
{
    int Q;
    scanf("%d", &Q);
    maxheap hp;
    for(int i=1;i<=30000;i++)
    {
        hp.pos[i] = -1;
    }
    hp.size = 0;
    for(int instruction_num = 0;instruction_num<=Q-1;instruction_num++)
    {
        int inst;
        scanf("%d", &inst);
        if(inst == 100)
        {
            scanf("%d %d", &N, &M);
            dist = (int*)malloc(sizeof(int) * N);
            map = (int**)malloc(sizeof(int*) * N);
            for(int i=0;i<=N-1;i++)
            {
                map[i] = (int*)malloc(sizeof(int) * N);
                for(int j=0;j<=N-1;j++)
                {
                    map[i][j] = MAX_DIST;
                }
                map[i][i] = 0;
            }
            for(int i=0;i<=M-1;i++)
            {
                int v, u, w;
                scanf("%d %d %d", &v, &u, &w);
                if(map[u][v] > w)
                {
                    map[u][v] = w;
                    map[v][u] = w;
                }
            }
            dijkstra(0);
        }
        else if(inst == 200) // 상품 생성
        {
            int id, revenue, dest;
            scanf("%d %d %d", &id, &revenue, &dest);
            product newproduct;
            newproduct.id = id;
            newproduct.revenue = revenue;
            newproduct.destination = dest;
            newproduct.suik = revenue-dist[dest];
            push(&hp, newproduct);
        }
        else if(inst == 300) // 상품 취소
        {
            int id;
            scanf("%d", &id);
            if(hp.pos[id] != -1)
            {
                pop(&hp, hp.pos[id]);
            }
        }
        else if(inst == 400) // 최적의 상품 출력
        {
            if(hp.size == 0 || hp.arr[1].suik < 0)
            {
                printf("-1\n");
            }
            else
            {
                product bestproduct = pop(&hp, 1);
                printf("%d\n", bestproduct.id);
            }
        }
        else if(inst == 500)
        {
            int s;
            scanf("%d", &s);
            dijkstra(s);
            int heap_size = hp.size;
            product* tempproduct = (product*)malloc(sizeof(product) * (heap_size+1));
            for(int i=1;i<=heap_size;i++)
            {
                tempproduct[i] = pop(&hp, 1);
                tempproduct[i].suik = tempproduct[i].revenue - dist[tempproduct[i].destination];
            }
            for(int i=1;i<=heap_size;i++)
            {
                push(&hp, tempproduct[i]);
            }
            free(tempproduct);
        }
    }
    for(int i=0;i<=N-1;i++)
    {
        free(map[i]);
    }
    free(map);
    free(dist);
    return 0;
}