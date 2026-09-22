#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

#define ALLOC_SIZE (64 * 1024 * 1024)
#define PAGE_STRIDE 4096

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Allocates a large buffer and touches one byte per page, then
 *         reports minor page faults via getrusage() before and after. Each
 *         first touch of a fresh page triggers a minor fault (no disk I/O
 *         needed - the kernel just backs the page with a zeroed physical
 *         frame), illustrating demand paging: malloc() reserves address
 *         space but does not commit physical memory until it is touched.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    struct rusage before;
    struct rusage after;
    char *buffer;
    size_t i;

    getrusage(RUSAGE_SELF, &before);

    buffer = malloc(ALLOC_SIZE);
    if (buffer == NULL)
    {
        fprintf(stderr, "malloc failed\n");
        return 1;
    }

    // Touch exactly one byte per page - enough to force the kernel to back
    // each page with physical memory, without wastefully writing every byte.
    for (i = 0; i < ALLOC_SIZE; i += PAGE_STRIDE)
    {
        buffer[i] = 1;
    }

    getrusage(RUSAGE_SELF, &after);

    printf("minor page faults before touching: %ld\n", before.ru_minflt);
    printf("minor page faults after touching:  %ld\n", after.ru_minflt);
    printf("minor faults caused by this loop:  %ld (expect roughly %d)\n",
           after.ru_minflt - before.ru_minflt, (int)(ALLOC_SIZE / PAGE_STRIDE));

    free(buffer);
    return 0;
}
