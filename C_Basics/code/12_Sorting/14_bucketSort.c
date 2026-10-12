// Bucket sort for uniformly distributed values: scatter into buckets,
// insertion-sort each, concatenate (expected O(n)).

#include <stdio.h>
#include <stdlib.h>

#define BUCKETS 5

typedef struct Node { double v; struct Node *next; } Node;

static void insert_sorted(Node **head, double v)
{
    Node *n = malloc(sizeof *n);
    n->v = v;
    while (*head && (*head)->v < v) head = &(*head)->next;   /* insertion keeps each bucket sorted */
    n->next = *head;
    *head = n;
}

int main(void)
{
    double a[] = {0.78, 0.17, 0.39, 0.26, 0.72, 0.94, 0.21, 0.12, 0.23, 0.68, 0.05, 0.99};
    int n = sizeof a / sizeof a[0];
    Node *bucket[BUCKETS] = {0};
    for (int i = 0; i < n; i++)
        insert_sorted(&bucket[(int)(a[i] * BUCKETS)], a[i]);  /* values in [0,1): bucket index = floor(v * BUCKETS) */

    int k = 0;
    for (int b = 0; b < BUCKETS; b++)
    {
        printf("bucket %d:", b);
        for (Node *p = bucket[b]; p; p = p->next) printf(" %.2f", p->v);
        printf("\n");
        for (Node *p = bucket[b]; p;)
        {
            a[k++] = p->v;
            Node *t = p->next;
            free(p);
            p = t;
        }
    }
    printf("sorted:");
    for (int i = 0; i < n; i++) printf(" %.2f", a[i]);
    printf("\n");
    return 0;
}
