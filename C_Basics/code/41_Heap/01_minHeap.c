#include <stdio.h>

#define MAX_HEAP_SIZE 128

typedef struct
{
    int data[MAX_HEAP_SIZE];
    int size;
} MinHeap;

void initHeap(MinHeap *heap)
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
void push(MinHeap *heap, int value)
{
    int index = heap->size;

    heap->data[index] = value;
    heap->size++;

    // Bubble up: swap with the parent as long as this node is smaller,
    // restoring the min-heap property (every parent <= its children).
    while ((index > 0) && (heap->data[index] < heap->data[(index - 1) / 2]))
    {
        int parent = (index - 1) / 2;

        swap(&heap->data[index], &heap->data[parent]);
        index = parent;
    }
}

int pop(MinHeap *heap)
{
    int minValue = heap->data[0];
    int index = 0;

    heap->size--;
    heap->data[0] = heap->data[heap->size];

    // Bubble down: swap with the smaller child until this node is no
    // longer larger than either child.
    while (1)
    {
        int left = (2 * index) + 1;
        int right = (2 * index) + 2;
        int smallest = index;

        if ((left < heap->size) && (heap->data[left] < heap->data[smallest]))
        {
            smallest = left;
        }
        if ((right < heap->size) && (heap->data[right] < heap->data[smallest]))
        {
            smallest = right;
        }
        if (smallest == index)
        {
            break;
        }

        swap(&heap->data[index], &heap->data[smallest]);
        index = smallest;
    }

    return minValue;
}

int main()
{
    MinHeap heap;
    int values[] = {5, 3, 8, 1, 9, 2};
    int size = sizeof(values) / sizeof(values[0]);
    int i;

    initHeap(&heap);
    for (i = 0; i < size; i++)
    {
        push(&heap, values[i]);
    }

    printf("popped in ascending order: ");
    while (heap.size > 0)
    {
        printf("%d ", pop(&heap));
    }
    printf("\n");

    return 0;
}
