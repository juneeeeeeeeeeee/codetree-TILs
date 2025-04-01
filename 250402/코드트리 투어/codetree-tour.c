#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_DIST 200000
typedef struct _product{
    int id;
    int suik;
    int revenue;
    int destination;
} product;
typedef struct _maxheap{
    struct _product arr[30000];
    int size;
} maxheap;
typedef struct _maxheap_int{
    int arr[2000];
    int size;
} maxheap_int;
typedef struct _ll_node{
    int n;
    int dist;
    struct _ll_node* next;
} ll_node;
ll_node** map;
int dist[2000];
int N;
bool is_alive[30000] = {false};
void push(maxheap* hp, product element)
{
    hp->size++;
    int findingpos = hp->size;
    while(findingpos != 1 && (element.suik > hp->arr[findingpos >> 1].suik || (element.suik == hp->arr[findingpos >> 1].suik && element.id < hp->arr[findingpos >> 1].id)))
    {
        hp->arr[findingpos] = hp->arr[findingpos >> 1];
        findingpos = findingpos >> 1;
    }
    hp->arr[findingpos] = element;
}

void push_int(maxheap_int* hp, int n)
{
    hp->size++;
    int findingpos = hp->size;
    while(findingpos != 1 && dist[n] > dist[hp->arr[findingpos >> 1]])
    {
        hp->arr[findingpos] = hp->arr[findingpos >> 1];
        findingpos = findingpos >> 1;
    }
    hp->arr[findingpos] = n;
}

product pop(maxheap* hp)
{
    product r = hp->arr[1];
    product last = hp->arr[hp->size--];
    if(!hp->size) return r;
    int parent = 1;
    int child = parent << 1;
    while(child <= hp->size)
    {
        if(child < hp->size && ((hp->arr[child].suik < hp->arr[child+1].suik) || (hp->arr[child].suik == hp->arr[child+1].suik && hp->arr[child].id>hp->arr[child+1].id)))
            child++;
        if(hp->arr[child].suik < last.suik || ((hp->arr[child].suik == last.suik) && hp->arr[child].id > last.id))
            break;
        hp->arr[parent] = hp->arr[child];
        parent = child;
        child = child<<1;
    }
    hp->arr[parent] = last;
    return r;
}

int pop_int(maxheap_int* hp)
{
    int r = hp->arr[1];
    int last = hp->arr[hp->size--];
    if(!hp->size) return r;
    int parent = 1;
    int child = parent << 1;
    while(child <= hp->size)
    {
        if(child < hp->size && dist[hp->arr[child]] < dist[hp->arr[child+1]])
            child++;
        if(dist[hp->arr[child]] < dist[last])
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
    maxheap_int hp;
    hp.size = 0;
    memset(dist, MAX_DIST, sizeof(int) * N);
    dist[source] = 0;
    push_int(&hp, source);
    while(hp.size)
    {
        int u = pop_int(&hp);
        ll_node* temp = map[u]->next;
        while(temp)
        {
            if(dist[u]+temp->dist < dist[temp->n])
            {
                dist[temp->n] = dist[u]+temp->dist;
                push_int(&hp, temp->n);
            }
            temp = temp->next;
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
    map = (ll_node**)malloc(sizeof(ll_node*) * N);
    for(int i=0;i<N;i++)
    {
        map[i] = (ll_node*)malloc(sizeof(ll_node));
        map[i]->n = 0;
        map[i]->next = NULL;
    }
    while(M--)
    {
        int v, u, w;
        scanf("%d %d %d", &v, &u, &w);
        if(u == v) continue;
        ll_node* temp = map[v];
        while(temp->next && temp->next->n < u)
            temp = temp->next;
        if(temp->next && temp->next->n == u)
        {
            if(temp->next->dist > w) temp->next->dist = w;
        }
        else
        {
            ll_node* new = (ll_node*)malloc(sizeof(ll_node));
            new->n = u;
            new->dist = w;
            new->next = temp->next;
            temp->next = new;
        }
        temp = map[u];
        while(temp->next && temp->next->n < v)
            temp = temp->next;
        if(temp->next && temp->next->n == v)
        {
            if(temp->next->dist > w) temp->next->dist = w;
        }
        else
        {
            ll_node* new2 = (ll_node*)malloc(sizeof(ll_node));
            new2->n = v;
            new2->dist = w;
            new2->next = temp->next;
            temp->next = new2;
        }
    }
    /*
    for(int i=0;i<N;i++)
    {
        printf("%d: ", i);
        ll_node* temp = map[i]->next;
        while(temp)
        {
            printf("%d %d ", temp->n, temp->dist);
            temp = temp->next;
        }
        printf("\n");
    }
    */
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
    for(int i=0;i<N;i++)
    {
        while(map[i]->next)
        {
            ll_node* temp = map[i]->next;
            ll_node* temptemp = temp->next;
            free(temp);
            map[i]->next = temptemp;
        }
        free(map[i]);
    }
    free(map);
    return 0;
}