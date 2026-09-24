#include <stdio.h>
#include <stdint.h>

// Illustrates the bit arithmetic from the NOTES.md walkthrough: splitting a
// virtual address into (page number, offset) for a hypothetical page size,
// then reassembling a physical address from a frame number + offset. This
// is the same shift/mask logic real MMUs perform in hardware.

#define PAGE_SIZE_BYTES 4096
#define OFFSET_BITS 12
#define OFFSET_MASK (PAGE_SIZE_BYTES - 1)

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Decomposes a sample virtual address into its page number and
 *         offset fields, then reassembles a physical address from a
 *         (frame number, offset) pair, mirroring the split/combine steps
 *         an MMU performs on every memory access.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    uint64_t virtualAddress;
    uint64_t pageNumber;
    uint64_t offset;
    uint64_t frameNumber;
    uint64_t physicalAddress;

    virtualAddress = 0x1A2B3C4D;
    pageNumber = virtualAddress >> OFFSET_BITS;
    offset = virtualAddress & OFFSET_MASK;

    printf("virtual address  = 0x%lx\n", virtualAddress);
    printf("page number      = 0x%lx\n", pageNumber);
    printf("offset           = 0x%lx\n", offset);

    // Pretend the page table walk (or a TLB hit) resolved pageNumber to
    // this frame number - the offset is carried through unchanged.
    frameNumber = 0x77;
    physicalAddress = (frameNumber << OFFSET_BITS) | offset;

    printf("frame number     = 0x%lx\n", frameNumber);
    printf("physical address = 0x%lx\n", physicalAddress);

    return 0;
}
