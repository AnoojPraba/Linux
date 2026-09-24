#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 16
#define MAX_KEY_LEN 64

// Separate chaining: each bucket is a singly linked list of entries, same
// node-append/traverse pattern as 33_LinkedList/01_singlyLinkedList.c -
// collisions just grow the list instead of overwriting the slot.
typedef struct Entry
{
    char key[MAX_KEY_LEN];
    int value;
    struct Entry *next;
} Entry;

typedef struct
{
    Entry *buckets[TABLE_SIZE];
} HashTable;

void initTable(HashTable *table)
{
    int i;

    for (i = 0; i < TABLE_SIZE; i++)
    {
        table->buckets[i] = NULL;
    }
}

// djb2: a simple, well-distributed string hash (used here instead of a
// plain modulo since keys are strings, not integers) - multiplying the
// running hash by 33 and adding each byte spreads similar keys apart well.
unsigned int hashString(const char *key)
{
    unsigned int hash = 5381;
    int c;

    while ((c = *key++) != '\0')
    {
        hash = ((hash << 5) + hash) + (unsigned int) c;
    }
    return hash % TABLE_SIZE;
}

void insert(HashTable *table, const char *key, int value)
{
    unsigned int index = hashString(key);
    Entry *current = table->buckets[index];
    Entry *node;

    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            current->value = value;
            return;
        }
        current = current->next;
    }

    node = malloc(sizeof(Entry));
    strncpy(node->key, key, MAX_KEY_LEN - 1);
    node->key[MAX_KEY_LEN - 1] = '\0';
    node->value = value;
    node->next = table->buckets[index];
    table->buckets[index] = node;
}

int lookup(HashTable *table, const char *key, int *found)
{
    unsigned int index = hashString(key);
    Entry *current = table->buckets[index];

    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            *found = 1;
            return current->value;
        }
        current = current->next;
    }
    *found = 0;
    return 0;
}

void deleteKey(HashTable *table, const char *key)
{
    unsigned int index = hashString(key);
    Entry *current = table->buckets[index];
    Entry *prev = NULL;

    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (prev == NULL)
            {
                table->buckets[index] = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

int main()
{
    HashTable table;
    int found;
    int value;

    initTable(&table);
    insert(&table, "apple", 1);
    insert(&table, "banana", 2);
    insert(&table, "cherry", 3);

    value = lookup(&table, "banana", &found);
    printf("banana -> %s (%d)\n", found ? "found" : "missing", value);

    deleteKey(&table, "banana");
    value = lookup(&table, "banana", &found);
    printf("banana -> %s\n", found ? "found" : "missing after delete");

    return 0;
}
