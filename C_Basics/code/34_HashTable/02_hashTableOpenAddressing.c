#include <stdio.h>
#include <string.h>

#define TABLE_SIZE 16
#define MAX_KEY_LEN 64

// Slot states: EMPTY has never been used (probing can stop), DELETED is a
// tombstone left behind by deleteKey (probing must keep going through it,
// since a later key may have probed past it during insert).
typedef enum
{
    SLOT_EMPTY,
    SLOT_OCCUPIED,
    SLOT_DELETED
} SlotState;

typedef struct
{
    char key[MAX_KEY_LEN];
    int value;
    SlotState state;
} Slot;

typedef struct
{
    Slot slots[TABLE_SIZE];
} HashTable;

void initTable(HashTable *table)
{
    int i;

    for (i = 0; i < TABLE_SIZE; i++)
    {
        table->slots[i].state = SLOT_EMPTY;
    }
}

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

// Linear probing: on collision, step to the next slot (wrapping around)
// until an empty/matching slot is found. Skips (but doesn't stop at)
// tombstones from deleteKey so lookups for keys probed past them still work.
void insert(HashTable *table, const char *key, int value)
{
    unsigned int index = hashString(key);
    int i;

    for (i = 0; i < TABLE_SIZE; i++)
    {
        unsigned int probe = (index + i) % TABLE_SIZE;

        if ((table->slots[probe].state != SLOT_OCCUPIED) ||
            (strcmp(table->slots[probe].key, key) == 0))
        {
            strncpy(table->slots[probe].key, key, MAX_KEY_LEN - 1);
            table->slots[probe].key[MAX_KEY_LEN - 1] = '\0';
            table->slots[probe].value = value;
            table->slots[probe].state = SLOT_OCCUPIED;
            return;
        }
    }
}

int lookup(HashTable *table, const char *key, int *found)
{
    unsigned int index = hashString(key);
    int i;

    for (i = 0; i < TABLE_SIZE; i++)
    {
        unsigned int probe = (index + i) % TABLE_SIZE;

        if (table->slots[probe].state == SLOT_EMPTY)
        {
            break;
        }
        if ((table->slots[probe].state == SLOT_OCCUPIED) &&
            (strcmp(table->slots[probe].key, key) == 0))
        {
            *found = 1;
            return table->slots[probe].value;
        }
    }
    *found = 0;
    return 0;
}

// Deleting must leave a tombstone (SLOT_DELETED), not reset to SLOT_EMPTY -
// clearing it outright would break the probe chain for any other key that
// collided here and was pushed further down the probe sequence.
void deleteKey(HashTable *table, const char *key)
{
    unsigned int index = hashString(key);
    int i;

    for (i = 0; i < TABLE_SIZE; i++)
    {
        unsigned int probe = (index + i) % TABLE_SIZE;

        if (table->slots[probe].state == SLOT_EMPTY)
        {
            return;
        }
        if ((table->slots[probe].state == SLOT_OCCUPIED) &&
            (strcmp(table->slots[probe].key, key) == 0))
        {
            table->slots[probe].state = SLOT_DELETED;
            return;
        }
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

    deleteKey(&table, "banana");
    value = lookup(&table, "cherry", &found);
    printf("cherry -> %s (%d)\n", found ? "found" : "missing", value);

    value = lookup(&table, "banana", &found);
    printf("banana -> %s\n", found ? "found" : "missing after delete");

    return 0;
}
