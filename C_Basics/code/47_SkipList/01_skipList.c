#include <stdio.h>
#include <stdlib.h>

#define MAX_LEVEL 4
#define COIN_FLIP_THRESHOLD 2

typedef struct SkipNode
{
    int key;
    int level;
    struct SkipNode *forward[MAX_LEVEL];
} SkipNode;

typedef struct
{
    SkipNode *header;
    int level;
} SkipList;

/*****************************************************************************
 * Name: createSkipNode
 *
 * Description:
 *         Allocates a skip-list node with the given key and level, with all
 *         forward pointers initialized to NULL.
 *
 * Inputs:
 *         key   : key to store in the new node.
 *         level : number of forward pointers the node participates in.
 *
 * Returns:
 *         Pointer to the newly allocated node.
 *****************************************************************************/
SkipNode *createSkipNode(int key, int level)
{
    SkipNode *node = malloc(sizeof(SkipNode));
    int i;

    node->key = key;
    node->level = level;
    for (i = 0; i < MAX_LEVEL; i++)
    {
        node->forward[i] = NULL;
    }
    return node;
}

/*****************************************************************************
 * Name: skipListInit
 *
 * Description:
 *         Initializes an empty skip list with a sentinel header node.
 *
 * Inputs:
 *         list : skip list to initialize.
 *
 * Returns:
 *         None
 *****************************************************************************/
void skipListInit(SkipList *list)
{
    list->header = createSkipNode(0, MAX_LEVEL - 1);
    list->level = 0;
}

/*****************************************************************************
 * Name: randomLevel
 *
 * Description:
 *         Picks a random level for a new node via repeated coin flips
 *         (geometric distribution): each successful flip climbs one more
 *         level, capped at MAX_LEVEL - 1.
 *
 * Returns:
 *         Randomly chosen level in [0, MAX_LEVEL - 1].
 *****************************************************************************/
int randomLevel(void)
{
    int level = 0;

    while (((rand() % COIN_FLIP_THRESHOLD) == 0) && (level < MAX_LEVEL - 1))
    {
        level = level + 1;
    }
    return level;
}

/*****************************************************************************
 * Name: skipListInsert
 *
 * Description:
 *         Inserts a key into the skip list, first recording the last node
 *         at each level whose key is less than the new key, then splicing
 *         the new node's forward pointers in at a randomly chosen level.
 *
 * Inputs:
 *         list : skip list to insert into.
 *         key  : key to insert.
 *
 * Returns:
 *         None
 *****************************************************************************/
void skipListInsert(SkipList *list, int key)
{
    SkipNode *update[MAX_LEVEL];
    SkipNode *current = list->header;
    int i;
    int newLevel;

    for (i = list->level; i >= 0; i--)
    {
        while ((current->forward[i] != NULL) && (current->forward[i]->key < key))
        {
            current = current->forward[i];
        }
        update[i] = current;
    }

    newLevel = randomLevel();
    if (newLevel > list->level)
    {
        for (i = list->level + 1; i <= newLevel; i++)
        {
            update[i] = list->header;
        }
        list->level = newLevel;
    }

    current = createSkipNode(key, newLevel);
    for (i = 0; i <= newLevel; i++)
    {
        current->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = current;
    }
}

/*****************************************************************************
 * Name: skipListSearch
 *
 * Description:
 *         Searches for a key by dropping down levels whenever the next
 *         node at the current level would overshoot the target.
 *
 * Inputs:
 *         list : skip list to search.
 *         key  : key to search for.
 *
 * Returns:
 *         1 if found, 0 otherwise.
 *****************************************************************************/
int skipListSearch(const SkipList *list, int key)
{
    SkipNode *current = list->header;
    int i;

    for (i = list->level; i >= 0; i--)
    {
        while ((current->forward[i] != NULL) && (current->forward[i]->key < key))
        {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    return (current != NULL) && (current->key == key);
}

/*****************************************************************************
 * Name: skipListDelete
 *
 * Description:
 *         Deletes a key from the skip list by re-linking forward pointers
 *         at every level the target node participates in.
 *
 * Inputs:
 *         list : skip list to delete from.
 *         key  : key to delete.
 *
 * Returns:
 *         None
 *****************************************************************************/
void skipListDelete(SkipList *list, int key)
{
    SkipNode *update[MAX_LEVEL];
    SkipNode *current = list->header;
    SkipNode *target;
    int i;

    for (i = list->level; i >= 0; i--)
    {
        while ((current->forward[i] != NULL) && (current->forward[i]->key < key))
        {
            current = current->forward[i];
        }
        update[i] = current;
    }

    target = current->forward[0];
    if ((target == NULL) || (target->key != key))
    {
        return;
    }

    for (i = 0; i <= list->level; i++)
    {
        if (update[i]->forward[i] != target)
        {
            continue;
        }
        update[i]->forward[i] = target->forward[i];
    }
    free(target);
}

int main(void)
{
    SkipList list;
    int values[] = { 3, 6, 7, 9, 12, 19, 17 };
    int count = sizeof(values) / sizeof(values[0]);
    int i;

    skipListInit(&list);
    for (i = 0; i < count; i++)
    {
        skipListInsert(&list, values[i]);
    }

    printf("search 19: %d\n", skipListSearch(&list, 19));
    printf("search 5: %d\n", skipListSearch(&list, 5));

    skipListDelete(&list, 19);
    printf("search 19 after delete: %d\n", skipListSearch(&list, 19));

    return 0;
}
