// Huffman coding: build the optimal prefix code with a min-heap of subtrees,
// print the codes and the compression ratio.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    int freq;
    char ch;
    struct Node *left, *right;
} Node;

static Node *heap[64];
static int hn;

static void push(Node *x)
{
    int i = hn++;
    heap[i] = x;
    while (i && heap[(i - 1) / 2]->freq > heap[i]->freq)
    {
        Node *t = heap[i]; heap[i] = heap[(i - 1) / 2]; heap[(i - 1) / 2] = t;
        i = (i - 1) / 2;
    }
}

static Node *pop(void)
{
    Node *top = heap[0];
    heap[0] = heap[--hn];
    for (int i = 0;;)
    {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < hn && heap[l]->freq < heap[m]->freq) m = l;
        if (r < hn && heap[r]->freq < heap[m]->freq) m = r;
        if (m == i) break;
        Node *t = heap[i]; heap[i] = heap[m]; heap[m] = t;
        i = m;
    }
    return top;
}

static char codes[256][32];

static void walk(Node *n, char *buf, int depth)
{
    if (!n->left && !n->right)
    {
        buf[depth] = '\0';
        strcpy(codes[(unsigned char)n->ch], depth ? buf : "0");
        return;
    }
    buf[depth] = '0'; walk(n->left, buf, depth + 1);
    buf[depth] = '1'; walk(n->right, buf, depth + 1);
}

static void destroy(Node *n)
{
    if (n) { destroy(n->left); destroy(n->right); free(n); }
}

int main(void)
{
    const char *text = "abracadabra alakazam";
    int freq[256] = {0};
    for (const char *p = text; *p; p++) freq[(unsigned char)*p]++;
    for (int c = 0; c < 256; c++)
        if (freq[c])
        {
            Node *n = calloc(1, sizeof *n);
            n->freq = freq[c];
            n->ch = (char)c;
            push(n);
        }
    while (hn > 1)                                     /* merge the two rarest subtrees */
    {
        Node *a = pop(), *b = pop(), *p = calloc(1, sizeof *p);
        p->freq = a->freq + b->freq;
        p->left = a;
        p->right = b;
        push(p);
    }
    Node *root = pop();
    char buf[32];
    walk(root, buf, 0);

    long bits = 0;
    for (int c = 0; c < 256; c++)
        if (freq[c])
        {
            printf("'%c' freq %2d code %s\n", c == ' ' ? '_' : c, freq[c], codes[c]);
            bits += (long)freq[c] * (long)strlen(codes[c]);
        }
    long fixed = (long)strlen(text) * 8;
    printf("encoded size = %ld bits instead of %ld (%.1f%%)\n", bits, fixed, 100.0 * bits / fixed);
    destroy(root);
    return 0;
}
