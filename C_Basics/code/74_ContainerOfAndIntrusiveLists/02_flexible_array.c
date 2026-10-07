#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Flexible array member (C99): a trailing unsized array lets header and
// payload live in ONE allocation - one malloc, one free, better locality.
struct packet
{
    unsigned short len;
    unsigned char data[];   // not counted in sizeof
};

static struct packet *packet_new(const void *src, unsigned short len)
{
    struct packet *p = malloc(sizeof *p + len);
    if (!p)
        return NULL;
    p->len = len;
    memcpy(p->data, src, len);
    return p;
}

int main(void)
{
    printf("sizeof(struct packet) = %zu (flexible member excluded)\n",
           sizeof(struct packet));

    struct packet *p = packet_new("hello", 6);
    if (!p)
        return 1;
    printf("len=%u data=%s\n", p->len, (char *)p->data);
    free(p);

    // Old pre-C99 hack was `data[1]` or `data[0]` (GNU): sizeof included the
    // dummy element and indexing past it was technically UB. The C99 form is
    // the standard-blessed version.
    return 0;
}
