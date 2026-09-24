#include <iostream>
#include <list>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <vector>
#include <functional>

#define SHARD_COUNT 4
#define SHARD_CAPACITY 2
#define WORKER_COUNT 4
#define OPS_PER_WORKER 8

using namespace std;

// Cross-reference: C_Basics/code/37_LRUCache/01_lruCacheDllHashMap.c builds a
// single-threaded O(1) LRU cache from a hand-rolled doubly-linked list plus a
// hash map (key -> list node). This file reuses that same O(1) idea (here via
// std::list + std::unordered_map, since this is a C++ repo) but shards it
// across SHARD_COUNT independent, individually-mutex-protected caches --
// "lock striping" -- so threads touching different shards never contend.
//
// Interview tradeoff: this buys concurrency at the cost of a single global
// recency order. Two keys that hash into different shards have their
// recency tracked completely independently, so "least recently used
// overall" is not a meaningful concept anymore -- only "least recently
// used within this shard" is. A true global-LRU thread-safe cache would
// need one lock (or a much more elaborate lock-free structure), which
// reintroduces the contention this design is trying to avoid.
class LockStripedLruCache
{
    private:
        struct Shard
        {
            std::mutex mutex;
            list<pair<int, int>> recencyList;
            unordered_map<int, list<pair<int, int>>::iterator> lookup;
        };

        vector<unique_ptr<Shard>> shards;
        size_t capacityPerShard;

        /*****************************************************************************
         * Name: shardFor
         *
         * Description:
         *         Selects which shard owns a given key via hash(key) % shard count.
         *
         * Inputs:
         *         key : the key being looked up or inserted.
         *
         * Returns:
         *         Reference to the owning shard.
         *****************************************************************************/
        Shard &shardFor(int key)
        {
            size_t index = hash<int>{}(key) % shards.size();
            return *shards[index];
        }

    public:
        /*****************************************************************************
         * Name: LockStripedLruCache
         *
         * Description:
         *         Constructs a cache with a fixed number of independently-locked
         *         shards, each with its own capacity.
         *
         * Inputs:
         *         shardCount    : number of independent lock-striped shards.
         *         perShardLimit : max entries in each shard before eviction.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        LockStripedLruCache(size_t shardCount, size_t perShardLimit)
            : capacityPerShard(perShardLimit)
        {
            for (size_t i = 0; i < shardCount; i = i + 1)
            {
                shards.push_back(make_unique<Shard>());
            }
        }

        /*****************************************************************************
         * Name: put
         *
         * Description:
         *         Inserts or updates a key/value pair, marking it most recently
         *         used within its shard and evicting that shard's least recently
         *         used entry if the shard is over capacity.
         *
         * Inputs:
         *         key   : the key to insert or update.
         *         value : the value to associate with the key.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void put(int key, int value)
        {
            Shard &shard = shardFor(key);
            lock_guard<mutex> lock(shard.mutex);

            auto found = shard.lookup.find(key);
            if (found != shard.lookup.end())
            {
                shard.recencyList.erase(found->second);
                shard.lookup.erase(found);
            }

            shard.recencyList.push_front({key, value});
            shard.lookup[key] = shard.recencyList.begin();

            if (shard.recencyList.size() > capacityPerShard)
            {
                auto &leastRecentlyUsed = shard.recencyList.back();
                shard.lookup.erase(leastRecentlyUsed.first);
                shard.recencyList.pop_back();
            }
        }

        /*****************************************************************************
         * Name: get
         *
         * Description:
         *         Looks up a key, marking it most recently used within its shard
         *         if found.
         *
         * Inputs:
         *         key       : the key to look up.
         *         outValue  : receives the value when the key is found.
         *
         * Returns:
         *         True if the key was found, false otherwise.
         *****************************************************************************/
        bool get(int key, int &outValue)
        {
            Shard &shard = shardFor(key);
            lock_guard<mutex> lock(shard.mutex);

            auto found = shard.lookup.find(key);
            if (found == shard.lookup.end())
            {
                return false;
            }

            pair<int, int> entry = *found->second;
            shard.recencyList.erase(found->second);
            shard.recencyList.push_front(entry);
            shard.lookup[key] = shard.recencyList.begin();
            outValue = entry.second;
            return true;
        }
};

/*****************************************************************************
 * Name: hammerCache
 *
 * Description:
 *         Worker function that puts and gets a range of keys, used to exercise
 *         the cache concurrently from multiple threads.
 *
 * Inputs:
 *         cache      : the shared cache under test.
 *         workerId   : identifies which slice of keys this worker touches.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void hammerCache(LockStripedLruCache &cache, int workerId)
{
    for (int i = 0; i < OPS_PER_WORKER; i = i + 1)
    {
        int key = workerId * OPS_PER_WORKER + i;
        cache.put(key, key * key);
    }

    for (int i = 0; i < OPS_PER_WORKER; i = i + 1)
    {
        int key = workerId * OPS_PER_WORKER + i;
        int value = 0;
        if (cache.get(key, value))
        {
            cout << "worker " << workerId << ": key " << key
                       << " -> " << value << "\n";
        }
    }
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Launches multiple worker threads that concurrently put/get keys
 *         into a lock-striped LRU cache.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    LockStripedLruCache cache(SHARD_COUNT, SHARD_CAPACITY);
    vector<thread> workers;

    for (int i = 0; i < WORKER_COUNT; i = i + 1)
    {
        workers.push_back(thread(hammerCache, ref(cache), i));
    }

    for (auto &worker : workers)
    {
        worker.join();
    }

    return 0;
}
