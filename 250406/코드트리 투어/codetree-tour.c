#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
typedef struct _product{
    int id;
    int suik;
    int revenue;
    int destination;
} product;
typedef struct _minheap_node{
    int node;
    int fake_dist;
} minheap_node;
typedef struct _maxheap{
    struct _product arr[30000];
    int size;
} maxheap;
typedef struct _minheap_int{
    struct _minheap_node arr[20000];
    int size;
} minheap_int;
int dist[2000];
int map[2000][2000];
int N;
bool is_alive[30000] = {false};
void push(maxheap* hp, product element)
{
    int findingpos = ++hp->size;
    if(element.suik >= 0) // 수익이 0보다 작으면 정렬할 필요 없이 맨 끝에 붙이기만 하면 됨
    {
        while(findingpos != 1 && (element.suik > hp->arr[findingpos >> 1].suik || (element.suik == hp->arr[findingpos >> 1].suik && element.id < hp->arr[findingpos >> 1].id)))
        {
            hp->arr[findingpos] = hp->arr[findingpos >> 1];
            findingpos = findingpos >> 1;
        }
    }
    hp->arr[findingpos] = element;
}

void push_int(minheap_int* hp, int n, int fake_dist)
{
    int findingpos = ++hp->size;
    while(findingpos != 1 && fake_dist < hp->arr[findingpos >> 1].fake_dist)
    {
        hp->arr[findingpos] = hp->arr[findingpos >> 1];
        findingpos = findingpos >> 1;
    }
    hp->arr[findingpos].node = n;
    hp->arr[findingpos].fake_dist = fake_dist;
}

product pop(maxheap* hp)
{
    product r = hp->arr[1];
    product last = hp->arr[hp->size--];
    while(hp->size && !is_alive[last.id])
    {
        last = hp->arr[hp->size--];
    }
    if(!hp->size) return r;
    int parent = 1;
    int child = 2;
    while(child <= hp->size)
    {
        if(child < hp->size && ((hp->arr[child].suik < hp->arr[child+1].suik) || (hp->arr[child].suik == hp->arr[child+1].suik && hp->arr[child].id>hp->arr[child+1].id)))
            child++;
        if(hp->arr[child].suik < 0 || (hp->arr[child].suik < last.suik || ((hp->arr[child].suik == last.suik) && hp->arr[child].id > last.id)))
            break;
        hp->arr[parent] = hp->arr[child];
        parent = child;
        child = child<<1;
    }
    hp->arr[parent] = last;
    return r;
}

minheap_node pop_int(minheap_int* hp)
{
    minheap_node r = hp->arr[1];
    minheap_node last = hp->arr[hp->size--];
    if(!hp->size) return r;
    int parent = 1;
    int child = 2;
    while(child <= hp->size)
    {
        if(child < hp->size && hp->arr[child].fake_dist > hp->arr[child+1].fake_dist)
            child++;
        if(hp->arr[child].fake_dist > last.fake_dist)
            break;
        hp->arr[parent] = hp->arr[child];
        parent = child;
        child = child<<1;
    }
    hp->arr[parent] = last;
    return r;
}

void dijkstra(int source)
{
    bool check[2000];
    minheap_int hp;
    hp.size = 0;
    for(int i=0;i<N;i++)
    {
        dist[i] = INT_MAX;
        check[i] = false;
    }
    dist[source] = 0;
    push_int(&hp, source, 0);
    while(hp.size)
    {
        minheap_node u = pop_int(&hp);
        if(u.fake_dist > dist[u.node]) continue; // update 안된 값
        check[u.node] = true;
        for(int i=0;i<N;i++)
        {
            if(map[u.node][i] != INT_MAX && !check[i] && dist[u.node] + map[u.node][i] < dist[i])
            {
                dist[i] = dist[u.node] + map[u.node][i];
                push_int(&hp, i, dist[i]);
            }
        }
    }
}

int main(void)
{
    int Q, M;
    scanf("%d", &Q);
    maxheap hp;
    hp.size = 0;
    scanf("%*d"); // inst 100
    Q--;
    scanf("%d %d", &N, &M);
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            map[i][j] = INT_MAX;
        }
    }
    while(M--)
    {
        int v, u, w;
        scanf("%d %d %d", &v, &u, &w);
        if(u == v) continue;
        if(map[u][v] > w)
        {
            map[u][v] = w;
            map[v][u] = w;
        }
    }
    dijkstra(0);
    while(Q--)
    {
        int inst;
        scanf("%d", &inst);
        if(inst == 200) // 상품 생성
        {
            int id, revenue, dest;
            scanf("%d %d %d", &id, &revenue, &dest);
            product newproduct = {id, revenue-dist[dest], revenue, dest};
            is_alive[id] = true;
            push(&hp, newproduct);
        }
        else if(inst == 300) // 상품 취소
        {
            int id;
            scanf("%d", &id);
            is_alive[id] = false;
        }
        else if(inst == 400) // 최적의 상품 출력
        {
            while(1)
            {
                if(!hp.size || (is_alive[hp.arr[1].id] && hp.arr[1].suik<0))
                {
                    printf("-1\n");
                    break;
                }
                product bestproduct = pop(&hp);
                if(is_alive[bestproduct.id])
                {
                    printf("%d\n", bestproduct.id);
                    break;
                }
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
                tempproduct[i] = hp.arr[i]; // 굳이 pop할 필요 없음
                if(is_alive[tempproduct[i].id])
                    tempproduct[i].suik = tempproduct[i].revenue - dist[tempproduct[i].destination];
            }
            hp.size = 0;
            for(int i=1;i<=heap_size;i++)
            {
                if(is_alive[tempproduct[i].id])
                    push(&hp, tempproduct[i]);
            }
            free(tempproduct);
        }
    }
    return 0;
}