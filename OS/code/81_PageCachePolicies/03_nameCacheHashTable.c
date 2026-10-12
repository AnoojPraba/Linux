// dentry-cache style chained hash table with hit/miss counters.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUCKETS 8

typedef struct Entry
{
    char name[32];
    int inode;
    struct Entry *next;
} Entry;

static Entry *table[BUCKETS];
static int hits, misses;

static unsigned hash(const char *s)
{
    unsigned h = 5381;                                  /* djb2 hash */
    while (*s)
        h = h * 33 + (unsigned char)*s++;
    return h % BUCKETS;
}

static void add(const char *name, int inode)
{
    Entry *e = calloc(1, sizeof *e);
    snprintf(e->name, sizeof e->name, "%s", name);
    e->inode = inode;
    e->next = table[hash(name)];
    table[hash(name)] = e;
}

static int lookup(const char *name)
{
    for (Entry *e = table[hash(name)]; e; e = e->next)
        if (strcmp(e->name, name) == 0)
        {
            hits++;
            return e->inode;
        }
    misses++;
    return -1;                                          /* not cached: read the directory from disk */
}

int main(void)
{
    const char *names[] = {"passwd", "hosts", "fstab", "group", "shadow", "hostname", "resolv.conf"};
    for (int i = 0; i < 7; i++)
        add(names[i], 1000 + i);

    printf("passwd -> %d\n", lookup("passwd"));
    printf("fstab  -> %d\n", lookup("fstab"));
    printf("motd   -> %d (not in the cache)\n", lookup("motd"));
    printf("hits = %d, misses = %d\n", hits, misses);

    int longest = 0;
    for (int b = 0; b < BUCKETS; b++)
    {
        int len = 0;
        for (Entry *e = table[b]; e; e = e->next)
            len++;
        if (len > longest)
            longest = len;
    }
    printf("seven names in %d buckets, longest chain = %d\n", BUCKETS, longest);
    return 0;
}
