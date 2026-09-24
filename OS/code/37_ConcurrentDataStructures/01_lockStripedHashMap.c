#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define NUM_STRIPES 8
#define BUCKETS_PER_STRIPE 32
#define NUM_WORKER_THREADS 4
#define KEYS_PER_THREAD 5000

// Lock-striped concurrent hash map: instead of one global mutex guarding
// the whole map (which serializes every operation, even ones touching
// unrelated keys), the bucket array is split into NUM_STRIPES groups, each
// with its own mutex. Two operations only contend if their keys hash into
// the same stripe, so unrelated keys proceed in parallel.
typedef struct Node
{
    int key;
    int value;
    struct Node *next;
} Node;

typedef struct
{
    Node *buckets[NUM_STRIPES * BUCKETS_PER_STRIPE];
    pthread_mutex_t stripeMutexes[NUM_STRIPES];
} StripedHashMap;

/*****************************************************************************
 * Name: stripedHashMapInit
 *
 * Description:
 *         Initializes every bucket to empty and every stripe's mutex.
 *
 * Inputs:
 *         map : the map to initialize.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void stripedHashMapInit(StripedHashMap *map)
{
    int i;

    for (i = 0; i < (NUM_STRIPES * BUCKETS_PER_STRIPE); i++)
    {
        map->buckets[i] = NULL;
    }
    for (i = 0; i < NUM_STRIPES; i++)
    {
        pthread_mutex_init(&map->stripeMutexes[i], NULL);
    }
}

/*****************************************************************************
 * Name: stripedHashMapBucketIndex
 *
 * Description:
 *         Maps a key to its bucket index across the whole bucket array.
 *
 * Inputs:
 *         key : the key to hash.
 *
 * Returns:
 *         The bucket index for the key.
 *****************************************************************************/
int stripedHashMapBucketIndex(int key)
{
    unsigned int hash = (unsigned int)key;

    return (int)(hash % (NUM_STRIPES * BUCKETS_PER_STRIPE));
}

/*****************************************************************************
 * Name: stripedHashMapStripeIndex
 *
 * Description:
 *         Maps a bucket index to the stripe (and thus mutex) that guards
 *         it - a stripe owns a contiguous run of BUCKETS_PER_STRIPE
 *         buckets.
 *
 * Inputs:
 *         bucketIndex : the bucket index to map.
 *
 * Returns:
 *         The stripe index for the bucket.
 *****************************************************************************/
int stripedHashMapStripeIndex(int bucketIndex)
{
    return bucketIndex / BUCKETS_PER_STRIPE;
}

/*****************************************************************************
 * Name: stripedHashMapInsert
 *
 * Description:
 *         Inserts (or updates) key/value, locking only the single stripe
 *         that owns this key's bucket - operations on keys in other
 *         stripes can proceed concurrently.
 *
 * Inputs:
 *         map   : the map to insert into.
 *         key   : the key to insert.
 *         value : the value to associate with the key.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void stripedHashMapInsert(StripedHashMap *map, int key, int value)
{
    int bucketIndex = stripedHashMapBucketIndex(key);
    int stripeIndex = stripedHashMapStripeIndex(bucketIndex);
    Node *node;

    pthread_mutex_lock(&map->stripeMutexes[stripeIndex]);

    for (node = map->buckets[bucketIndex]; node != NULL; node = node->next)
    {
        if (node->key == key)
        {
            node->value = value;
            pthread_mutex_unlock(&map->stripeMutexes[stripeIndex]);
            return;
        }
    }

    node = malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->next = map->buckets[bucketIndex];
    map->buckets[bucketIndex] = node;

    pthread_mutex_unlock(&map->stripeMutexes[stripeIndex]);
}

/*****************************************************************************
 * Name: stripedHashMapLookup
 *
 * Description:
 *         Looks up a key, locking only the stripe that owns its bucket.
 *
 * Inputs:
 *         map      : the map to look up in.
 *         key      : the key to search for.
 *         outValue : receives the value on success.
 *
 * Returns:
 *         1 if found, 0 if not found.
 *****************************************************************************/
int stripedHashMapLookup(StripedHashMap *map, int key, int *outValue)
{
    int bucketIndex = stripedHashMapBucketIndex(key);
    int stripeIndex = stripedHashMapStripeIndex(bucketIndex);
    Node *node;
    int found = 0;

    pthread_mutex_lock(&map->stripeMutexes[stripeIndex]);

    for (node = map->buckets[bucketIndex]; node != NULL; node = node->next)
    {
        if (node->key == key)
        {
            *outValue = node->value;
            found = 1;
            break;
        }
    }

    pthread_mutex_unlock(&map->stripeMutexes[stripeIndex]);
    return found;
}

StripedHashMap sharedMap;

/*****************************************************************************
 * Name: workerThread
 *
 * Description:
 *         Inserts a disjoint range of keys into the shared map, then reads
 *         each of them back to confirm the value round-tripped correctly.
 *
 * Inputs:
 *         arg : worker thread index, passed as a pointer-sized integer.
 *
 * Returns:
 *         NULL always.
 *****************************************************************************/
void *workerThread(void *arg)
{
    long id = (long)arg;
    int base = (int)id * KEYS_PER_THREAD;
    int i;
    int mismatches = 0;

    for (i = 0; i < KEYS_PER_THREAD; i++)
    {
        stripedHashMapInsert(&sharedMap, base + i, (base + i) * 2);
    }
    for (i = 0; i < KEYS_PER_THREAD; i++)
    {
        int value;

        if ((stripedHashMapLookup(&sharedMap, base + i, &value) == 0) ||
            (value != (base + i) * 2))
        {
            mismatches++;
        }
    }

    printf("worker %ld finished, mismatches = %d\n", id, mismatches);
    return NULL;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Runs several worker threads inserting/looking up disjoint key
 *         ranges concurrently in the lock-striped map, confirming no data
 *         was lost or corrupted.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    pthread_t workers[NUM_WORKER_THREADS];
    long i;

    stripedHashMapInit(&sharedMap);

    for (i = 0; i < NUM_WORKER_THREADS; i++)
    {
        pthread_create(&workers[i], NULL, workerThread, (void *)i);
    }
    for (i = 0; i < NUM_WORKER_THREADS; i++)
    {
        pthread_join(workers[i], NULL);
    }

    printf("all workers done\n");
    return 0;
}
