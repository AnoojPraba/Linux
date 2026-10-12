// O(1) LRU: hash table to find the node, doubly linked list for recency, evict
// the tail.

#include <stdio.h>
#include <stdlib.h>

#define CAP 3
#define BUCKETS 8

typedef struct Page
{
    int number;
    struct Page *prev, *next;       /* order of use */
    struct Page *chain;             /* hash bucket chain */
} Page;

static Page *table[BUCKETS];
static Page *head, *tail;
static int size;

static Page *lookup(int n)
{
    for (Page *p = table[n % BUCKETS]; p; p = p->chain)
        if (p->number == n)
            return p;
    return NULL;
}

static void unlink_page(Page *p)
{
    if (p->prev) p->prev->next = p->next; else head = p->next;
    if (p->next) p->next->prev = p->prev; else tail = p->prev;
}

static void push_front(Page *p)
{
    p->prev = NULL;
    p->next = head;
    if (head) head->prev = p;
    head = p;
    if (!tail) tail = p;
}

static void hash_remove(Page *p)
{
    Page **pp = &table[p->number % BUCKETS];
    while (*pp != p)
        pp = &(*pp)->chain;
    *pp = p->chain;
}

static int access_page(int n)       /* returns 1 on a hit, 0 on a fault */
{
    Page *p = lookup(n);
    if (p)
    {
        unlink_page(p);
        push_front(p);              /* now the most recently used */
        return 1;
    }
    if (size == CAP)                /* full: evict the least recently used page */
    {
        Page *victim = tail;
        printf("  evict %d\n", victim->number);
        unlink_page(victim);
        hash_remove(victim);
        free(victim);
        size--;
    }
    p = calloc(1, sizeof *p);
    p->number = n;
    p->chain = table[n % BUCKETS];
    table[n % BUCKETS] = p;
    push_front(p);
    size++;
    return 0;
}

int main(void)
{
    int refs[] = {1, 2, 3, 1, 4, 2, 5, 1};
    int faults = 0;
    for (int i = 0; i < 8; i++)
    {
        int hit = access_page(refs[i]);
        faults += !hit;
        printf("access %d: %s\n", refs[i], hit ? "hit" : "fault");
    }
    printf("page faults = %d\n", faults);
    while (head)
    {
        Page *t = head->next;
        free(head);
        head = t;
    }
    return 0;
}
