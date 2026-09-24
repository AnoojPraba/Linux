#include <stdio.h>
#include <string.h>

#define TABLE_SIZE 16
#define MAX_KEY_LEN 64
#define DJB2_SEED 5381
#define DJB2_SHIFT 5

typedef struct
{
    char key[MAX_KEY_LEN];
    int value;
    int occupied;
    // Distance this entry currently sits from its ideal (hashed) slot -
    // what "Robin Hood" swaps equalize across occupied slots.
    int probeDistance;
} Slot;

typedef struct
{
    Slot slots[TABLE_SIZE];
} RobinHoodTable;

/*****************************************************************************
 * Name: initTable
 *
 * Description:
 *         Marks every slot in the table as unoccupied.
 *
 * Inputs:
 *         table : table to initialize.
 *
 * Returns:
 *         None
 *****************************************************************************/
void initTable(RobinHoodTable *table)
{
    int i;

    for (i = 0; i < TABLE_SIZE; i++)
    {
        table->slots[i].occupied = 0;
    }
}

/*****************************************************************************
 * Name: hashString
 *
 * Description:
 *         Classic djb2 string hash, folded into the table's slot range.
 *
 * Inputs:
 *         key : NUL-terminated string to hash.
 *
 * Returns:
 *         Ideal slot index in [0, TABLE_SIZE).
 *****************************************************************************/
unsigned int hashString(const char *key)
{
    unsigned int hash = DJB2_SEED;
    int c;

    while ((c = *key++) != '\0')
    {
        hash = ((hash << DJB2_SHIFT) + hash) + (unsigned int) c;
    }
    return hash % TABLE_SIZE;
}

/*****************************************************************************
 * Name: robinHoodInsert
 *
 * Description:
 *         Inserts a key/value pair via Robin Hood open addressing: probes
 *         linearly from the key's ideal slot, and whenever the entry
 *         currently occupying a candidate slot has probed a shorter
 *         distance than the entry being inserted, the two swap places
 *         ("rob from the rich, give to the poor") and probing continues
 *         with the displaced entry. This keeps probe-length variance low
 *         across the table instead of letting plain linear probing cluster.
 *
 * Inputs:
 *         table : table to modify.
 *         key   : NUL-terminated key to insert.
 *         value : value to associate with the key.
 *
 * Returns:
 *         None
 *****************************************************************************/
void robinHoodInsert(RobinHoodTable *table, const char *key, int value)
{
    unsigned int index = hashString(key);
    Slot incoming;
    int distance = 0;
    int i;

    strncpy(incoming.key, key, MAX_KEY_LEN - 1);
    incoming.key[MAX_KEY_LEN - 1] = '\0';
    incoming.value = value;
    incoming.occupied = 1;

    for (i = 0; i < TABLE_SIZE; i++)
    {
        unsigned int probe = (index + (unsigned int) distance) % TABLE_SIZE;
        Slot *slot = &table->slots[probe];

        if (!slot->occupied)
        {
            incoming.probeDistance = distance;
            *slot = incoming;
            return;
        }
        if (strcmp(slot->key, incoming.key) == 0)
        {
            slot->value = incoming.value;
            return;
        }
        if (slot->probeDistance < distance)
        {
            Slot displaced = *slot;

            incoming.probeDistance = distance;
            *slot = incoming;
            incoming = displaced;
            distance = displaced.probeDistance;
        }
        distance++;
    }
}

/*****************************************************************************
 * Name: robinHoodLookup
 *
 * Description:
 *         Looks up a key, probing linearly from its ideal slot and stopping
 *         early once an empty slot or a slot with a shorter probe distance
 *         than the current search distance is reached - at that point the
 *         key cannot be present further along, since Robin Hood insertion
 *         guarantees entries are ordered by non-decreasing probe distance
 *         along any probe sequence.
 *
 * Inputs:
 *         table : table to search.
 *         key   : NUL-terminated key to look for.
 *         found : out-param set to 1 if found, 0 otherwise.
 *
 * Returns:
 *         The associated value if found, 0 otherwise.
 *****************************************************************************/
int robinHoodLookup(const RobinHoodTable *table, const char *key, int *found)
{
    unsigned int index = hashString(key);
    int distance = 0;
    int i;

    for (i = 0; i < TABLE_SIZE; i++)
    {
        unsigned int probe = (index + (unsigned int) distance) % TABLE_SIZE;
        const Slot *slot = &table->slots[probe];

        if ((!slot->occupied) || (slot->probeDistance < distance))
        {
            break;
        }
        if (strcmp(slot->key, key) == 0)
        {
            *found = 1;
            return slot->value;
        }
        distance++;
    }
    *found = 0;
    return 0;
}

/*****************************************************************************
 * Name: printProbeDistances
 *
 * Description:
 *         Prints every occupied slot's key and probe distance, to
 *         demonstrate Robin Hood's equalizing effect on probe length.
 *
 * Inputs:
 *         table : table to print.
 *
 * Returns:
 *         None
 *****************************************************************************/
void printProbeDistances(const RobinHoodTable *table)
{
    int i;

    for (i = 0; i < TABLE_SIZE; i++)
    {
        if (table->slots[i].occupied)
        {
            printf("slot %2d: key=%-8s probeDistance=%d\n", i, table->slots[i].key,
                   table->slots[i].probeDistance);
        }
    }
}

int main(void)
{
    RobinHoodTable table;
    const char *keys[] = { "apple", "banana", "cherry", "date", "elderberry",
                            "fig", "grape", "honeydew", "kiwi", "lemon", "mango",
                            "nectarine" };
    int keyCount = sizeof(keys) / sizeof(keys[0]);
    int i;
    int found;
    int value;

    initTable(&table);
    for (i = 0; i < keyCount; i++)
    {
        robinHoodInsert(&table, keys[i], i * 10);
    }

    printf("Probe distances after inserting %d keys:\n", keyCount);
    printProbeDistances(&table);

    value = robinHoodLookup(&table, "cherry", &found);
    printf("lookup(cherry) = %s (%d)\n", found ? "found" : "missing", value);

    value = robinHoodLookup(&table, "papaya", &found);
    printf("lookup(papaya) = %s\n", found ? "found" : "missing");

    return 0;
}
