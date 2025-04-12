#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MINV 1
#define MAXV 1000000000
typedef struct _segnode{
    int count;
    long long sum;
    char* name; // leaf node만
    struct _segnode* left;
    struct _segnode* right;
} segnode; // segment tree
typedef struct _namenode{
    char name[11];
    int value;
    struct _namenode* left;
    struct _namenode* right;
} namenode; // BST
int delete_value;

int search(segnode* node, int l, int r, int value)
{
    if(!node) return 0;
    if(l == r) return node->count;
    int m = (l+r)>>1;
    if(value<=m) return search(node->left, l, m, value);
    else return search(node->right, m+1, r, value);
}

segnode* insert(segnode* node, int l, int r, int value, char* name)
{
    if(!node)
    {
        node = malloc(sizeof(segnode));
        node->count = 0;
        node->sum = 0;
        node->left = NULL;
        node->right = NULL;
        node->name = NULL;
    }
    node->count += 1;
    node->sum += value;
    if(l == r) 
    {
        node->name = (char*)malloc(sizeof(char) * 11);
        strcpy(node->name, name);
        return node;
    }
    int m = (l+r)>>1;
    if(value <= m) node->left = insert(node->left, l, m, value, name);
    else node->right = insert(node->right, m+1, r, value, name);
    return node;
}

segnode* delete(segnode* node, int l, int r, int value)
{
    if(!node) return NULL;
    node->count -= 1;
    node->sum -= value;
    if(l == r)
    {
        if(node->name)
        {
            free(node->name);
            node->name = NULL;
        }
        return node;
    }
    int m = (l+r)>>1;
    if(value<=m) node->left = delete(node->left, l, m, value);
    else node->right = delete(node->right, m+1, r, value);
    return node;
}

long long sum(segnode* node, int l, int r, int value)
{
    if(!node || l>value) return 0;
    if(r<=value) return node->sum;
    int m = (l+r)>>1;
    return sum(node->left, l, m, value) + sum(node->right, m+1, r, value);
}

void kth_leq(segnode* node, int l, int r, int k)
{
    if(!node || node->count < k) return;
    if(l == r)
    {
        printf("%s\n", node->name);
        return;
    }
    int m = (l+r)>>1;
    int left_count = node->left ? node->left->count : 0;
    if(k <= left_count) kth_leq(node->left, l, m, k);
    else kth_leq(node->right, m+1, r, k - left_count);
}

void seg_free(segnode* node)
{
    if(node->left) seg_free(node->left);
    if(node->right) seg_free(node->right);
    if(node->name) free(node->name);
    free(node);
}

int search_name(namenode* node, const char* name)
{
    if(!node) return 0;
    int compare = strcmp(node->name, name);
    if(compare > 0) return search_name(node->left, name);
    else if(compare < 0) return search_name(node->right, name);
    else return 1;
}

namenode* insert_name(namenode* node, const char* name, int value)
{
    if(!node)
    {
        node = malloc(sizeof(namenode));
        strcpy(node->name, name);
        node->value = value;
        node->left = NULL;
        node->right = NULL;
        return node;
    }
    int compare = strcmp(node->name, name);
    if(compare > 0) node->left = insert_name(node->left, name, value);
    else node->right = insert_name(node->right, name, value);
    return node;
}

namenode* delete_name(namenode* node, const char* name)
{
    if(!node) return NULL;
    int compare = strcmp(node->name, name);
    if(compare > 0) node->left = delete_name(node->left, name);
    else if(compare < 0) node->right = delete_name(node->right, name);
    else // 이름 찾음
    {
        delete_value = node->value;
        if(!node->left)
        {
            namenode* rightnode = node->right;
            free(node);
            return rightnode;
        }
        else if(!node->right)
        {
            namenode* leftnode = node->left;
            free(node);
            return leftnode;
        }
        else
        {
            namenode* temp = node->right; // temp는 오른쪽 트리에서 가장 작은 node
            while(temp->left) temp = temp->left;
            strcpy(node->name, temp->name);
            node->value = temp->value;
            node->right = delete_name(node->right, temp->name);
        }
    }
    return node;
}

void name_free(namenode* node)
{
    if(node->left) name_free(node->left);
    if(node->right) name_free(node->right);
    free(node);
}

int main(void)
{
    segnode* segroot = NULL;
    namenode* nameroot = NULL;
    int Q;
    scanf("%d", &Q);
    while(Q--)
    {
        char s[7];
        scanf("%s", s);
        if(!strcmp(s, "init"))
        {
            if(segroot)
            {
                seg_free(segroot);
                segroot = NULL;
            }
            if(nameroot) 
            {
                name_free(nameroot);
                nameroot = NULL;
            }
        }
        else if(!strcmp(s, "insert"))
        {
            char name[11];
            int value;
            scanf("%s %d", name, &value);
            if(!search(segroot, MINV, MAXV, value) && !search_name(nameroot, name))
            {
                segroot = insert(segroot, MINV, MAXV, value, name);
                nameroot = insert_name(nameroot, name, value);
                printf("1\n");
            }
            else printf("0\n");
        }
        else if(!strcmp(s, "delete"))
        {
            char name[11];
            scanf("%s", name);
            delete_value = 0;
            nameroot = delete_name(nameroot, name);
            if(!delete_value) printf("0\n");
            else
            {
                printf("%d\n", delete_value);
                segroot = delete(segroot, MINV, MAXV, delete_value);
            }
        }
        else if(!strcmp(s, "rank"))
        {
            int k;
            scanf("%d", &k);
            if(!segroot || segroot->count < k) printf("None\n");
            else kth_leq(segroot, MINV, MAXV, k);
        }
        else
        {
            int k;
            scanf("%d", &k);
            printf("%lld\n", sum(segroot, MINV, MAXV, k));
        }
    }
    seg_free(segroot);
    name_free(nameroot);
    return 0;
}