#include <stdio.h>
#include <stdlib.h>

#define CACHE_CAPACITY 3
#define HASH_TABLE_SIZE 8

// O(1) LRU cache: a doubly-linked list (DLL) maintains recency order (most
// recently used at head, least at tail) and a hash map (key -> DLL node
// pointer) gives O(1) lookup by key. See NOTES.md for why neither structure
// alone can give O(1) for both "find by key" and "mark as most recent".

typedef struct DllNode
{
    int key;
    int value;
    struct DllNode *prev;
    struct DllNode *next;
} DllNode;

typedef struct HashEntry
{
    int key;
    DllNode *node;
    struct HashEntry *next;
} HashEntry;

typedef struct
{
    DllNode *head;
    DllNode *tail;
    HashEntry *buckets[HASH_TABLE_SIZE];
    int size;
    int capacity;
} LruCache;

/*****************************************************************************
 * Name: hashKey
 *
 * Description:
 *         Maps an integer key into a hash bucket index.
 *
 * Inputs:
 *         key : the key to hash.
 *
 * Returns:
 *         Bucket index in [0, HASH_TABLE_SIZE).
 *****************************************************************************/
int hashKey(int key)
{
    unsigned int uKey = (unsigned int) key;
    return (int) (uKey % HASH_TABLE_SIZE);
}

/*****************************************************************************
 * Name: cacheInit
 *
 * Description:
 *         Initializes an empty LRU cache with a fixed capacity.
 *
 * Inputs:
 *         cache    : the cache to initialize.
 *         capacity : maximum number of entries before eviction kicks in.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void cacheInit(LruCache *cache, int capacity)
{
    int i;

    cache->head = NULL;
    cache->tail = NULL;
    cache->size = 0;
    cache->capacity = capacity;
    for (i = 0; i < HASH_TABLE_SIZE; i++)
    {
        cache->buckets[i] = NULL;
    }
}

/*****************************************************************************
 * Name: findEntry
 *
 * Description:
 *         Looks up the hash entry for a key, for O(1) node lookup.
 *
 * Inputs:
 *         cache : the cache to search.
 *         key   : the key to look up.
 *
 * Returns:
 *         Pointer to the matching hash entry, or NULL if not found.
 *****************************************************************************/
HashEntry *findEntry(LruCache *cache, int key)
{
    HashEntry *entry = cache->buckets[hashKey(key)];

    while (entry != NULL)
    {
        if (entry->key == key)
        {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

/*****************************************************************************
 * Name: detachNode
 *
 * Description:
 *         Unlinks a node from the DLL without freeing it.
 *
 * Inputs:
 *         cache : the cache whose DLL is being modified.
 *         node  : the node to detach.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void detachNode(LruCache *cache, DllNode *node)
{
    if (node->prev != NULL)
    {
        node->prev->next = node->next;
    }
    else
    {
        cache->head = node->next;
    }

    if (node->next != NULL)
    {
        node->next->prev = node->prev;
    }
    else
    {
        cache->tail = node->prev;
    }
}

/*****************************************************************************
 * Name: pushFront
 *
 * Description:
 *         Inserts a node at the head of the DLL (marks it most recently used).
 *
 * Inputs:
 *         cache : the cache whose DLL is being modified.
 *         node  : the node to insert at the head.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void pushFront(LruCache *cache, DllNode *node)
{
    node->prev = NULL;
    node->next = cache->head;
    if (cache->head != NULL)
    {
        cache->head->prev = node;
    }
    cache->head = node;
    if (cache->tail == NULL)
    {
        cache->tail = node;
    }
}

/*****************************************************************************
 * Name: cacheGet
 *
 * Description:
 *         Looks up a key's value in O(1) and moves its node to the head
 *         (most recently used) in O(1).
 *
 * Inputs:
 *         cache : the cache to query.
 *         key   : the key to look up.
 *         found : out-param set to 1 if the key was present, 0 otherwise.
 *
 * Returns:
 *         The value for key if found, otherwise an unspecified value.
 *****************************************************************************/
int cacheGet(LruCache *cache, int key, int *found)
{
    HashEntry *entry = findEntry(cache, key);

    if (entry == NULL)
    {
        *found = 0;
        return 0;
    }

    *found = 1;
    detachNode(cache, entry->node);
    pushFront(cache, entry->node);
    return entry->node->value;
}

/*****************************************************************************
 * Name: evictTail
 *
 * Description:
 *         Evicts the least-recently-used node (the DLL tail) and removes
 *         its hash map entry.
 *
 * Inputs:
 *         cache : the cache to evict from.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void evictTail(LruCache *cache)
{
    DllNode *victim = cache->tail;
    int bucket = hashKey(victim->key);
    HashEntry *entry = cache->buckets[bucket];
    HashEntry *prevEntry = NULL;

    while (entry != NULL)
    {
        if (entry->key == victim->key)
        {
            if (prevEntry == NULL)
            {
                cache->buckets[bucket] = entry->next;
            }
            else
            {
                prevEntry->next = entry->next;
            }
            free(entry);
            break;
        }
        prevEntry = entry;
        entry = entry->next;
    }

    detachNode(cache, victim);
    free(victim);
    cache->size--;
}

/*****************************************************************************
 * Name: cachePut
 *
 * Description:
 *         Inserts or updates a key/value pair in O(1), evicting the least
 *         recently used entry if the cache is at capacity.
 *
 * Inputs:
 *         cache : the cache to insert into.
 *         key   : the key to insert or update.
 *         value : the value to associate with key.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void cachePut(LruCache *cache, int key, int value)
{
    HashEntry *entry = findEntry(cache, key);
    DllNode *node;
    int bucket;

    if (entry != NULL)
    {
        entry->node->value = value;
        detachNode(cache, entry->node);
        pushFront(cache, entry->node);
        return;
    }

    if (cache->size == cache->capacity)
    {
        evictTail(cache);
    }

    node = malloc(sizeof(DllNode));
    node->key = key;
    node->value = value;
    pushFront(cache, node);

    bucket = hashKey(key);
    entry = malloc(sizeof(HashEntry));
    entry->key = key;
    entry->node = node;
    entry->next = cache->buckets[bucket];
    cache->buckets[bucket] = entry;

    cache->size++;
}

int main(void)
{
    LruCache cache;
    int found;
    int value;

    cacheInit(&cache, CACHE_CAPACITY);

    cachePut(&cache, 1, 100);
    cachePut(&cache, 2, 200);
    cachePut(&cache, 3, 300);

    value = cacheGet(&cache, 1, &found);
    printf("get(1) = %d (found=%d)\n", value, found);

    // Capacity is 3 and key 1 was just refreshed, so key 2 is now the least
    // recently used and gets evicted here.
    cachePut(&cache, 4, 400);

    value = cacheGet(&cache, 2, &found);
    printf("get(2) found=%d (expected evicted)\n", found);

    value = cacheGet(&cache, 4, &found);
    printf("get(4) = %d (found=%d)\n", value, found);

    return 0;
}
