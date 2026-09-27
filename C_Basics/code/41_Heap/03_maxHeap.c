#include <stdio.h>

#define MAX_HEAP_SIZE 128

typedef struct
{
    int data[MAX_HEAP_SIZE];
    int size;
} MaxHeap;

void initHeap(MaxHeap *heap)
{
    heap->size = 0;
}

void swap(int *a, int *b)
{
    int tmp = *a;

    *a = *b;
    *b = tmp;
}

// A binary heap stored in a flat array: for index i, children live at
// 2i + 1 and 2i + 2, and the parent lives at (i - 1) / 2 - no explicit
// left/right/parent pointers needed, unlike the BST in 38_BinaryTree/.
void push(MaxHeap *heap, int value)
{
    int index = heap->size;

    heap->data[index] = value;
    heap->size++;

    // Bubble up: swap with the parent as long as this node is larger,
    // restoring the max-heap property (every parent >= its children).
    while ((index > 0) && (heap->data[index] > heap->data[(index - 1) / 2]))
    {
        int parent = (index - 1) / 2;

        swap(&heap->data[index], &heap->data[parent]);
        index = parent;
    }
}

int extractMax(MaxHeap *heap)
{
    int maxValue = heap->data[0];
    int index = 0;

    heap->size--;
    heap->data[0] = heap->data[heap->size];

    // Bubble down: swap with the larger child until this node is no
    // longer smaller than either child.
    while (1)
    {
        int left = (2 * index) + 1;
        int right = (2 * index) + 2;
        int largest = index;

        if ((left < heap->size) && (heap->data[left] > heap->data[largest]))
        {
            largest = left;
        }
        if ((right < heap->size) && (heap->data[right] > heap->data[largest]))
        {
            largest = right;
        }
        if (largest == index)
        {
            break;
        }

        swap(&heap->data[index], &heap->data[largest]);
        index = largest;
    }

    return maxValue;
}

int peek(MaxHeap *heap)
{
    return heap->data[0];
}

int main()
{
    MaxHeap heap;
    int values[] = {5, 3, 8, 1, 9, 2};
    int size = sizeof(values) / sizeof(values[0]);
    int i;

    initHeap(&heap);
    for (i = 0; i < size; i++)
    {
        push(&heap, values[i]);
        printf("pushed %d, current max: %d\n", values[i], peek(&heap));
    }

    printf("extracted in descending order: ");
    while (heap.size > 0)
    {
        printf("%d ", extractMax(&heap));
    }
    printf("\n");

    return 0;
}
