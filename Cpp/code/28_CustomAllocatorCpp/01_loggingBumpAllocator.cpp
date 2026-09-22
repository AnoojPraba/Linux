#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <new>
#include <vector>

#define ARENA_BYTE_COUNT 4096

// A minimal allocator-aware-container-conforming allocator: a bump/arena allocator that
// hands out slices of one fixed backing buffer and logs every allocate/deallocate call so
// std::vector's allocation pattern (reservations, growth reallocations) is visible.
// Falls back to malloc when the arena is exhausted, so it stays usable for any size.
template <typename T>
class LoggingBumpAllocator
{
public:
    using value_type = T;

    LoggingBumpAllocator() noexcept
    {
    }

    // Converting constructor required so containers can rebind this allocator to an
    // internal node/value type different from T (e.g. list nodes).
    template <typename U>
    LoggingBumpAllocator(const LoggingBumpAllocator<U> &) noexcept
    {
    }

    /*****************************************************************************
     * Name: allocate
     *
     * Description:
     *         Serves memory for objectCount objects of type T from the shared
     *         static arena if there is room, otherwise falls back to malloc.
     *         Logs which path was taken.
     *
     * Inputs:
     *         objectCount : number of T objects the caller needs storage for.
     *
     * Returns:
     *         Pointer to storage large enough for objectCount objects of type T.
     *****************************************************************************/
    T *allocate(std::size_t objectCount)
    {
        std::size_t bytesNeeded = objectCount * sizeof(T);
        if (arenaOffset + bytesNeeded <= ARENA_BYTE_COUNT)
        {
            T *result = reinterpret_cast<T *>(&arena[arenaOffset]);
            arenaOffset = arenaOffset + bytesNeeded;
            std::cout << "  allocate " << bytesNeeded << " bytes from arena (offset now "
                      << arenaOffset << ")\n";
            return result;
        }

        std::cout << "  allocate " << bytesNeeded << " bytes via malloc (arena exhausted)\n";
        void *fallback = std::malloc(bytesNeeded);
        if (fallback == nullptr)
        {
            throw std::bad_alloc();
        }
        return reinterpret_cast<T *>(fallback);
    }

    /*****************************************************************************
     * Name: deallocate
     *
     * Description:
     *         Releases memory previously returned by allocate(). Arena-backed
     *         pointers are simply logged and left alone (the bump allocator
     *         never reclaims individual blocks); malloc-backed pointers are
     *         freed.
     *
     * Inputs:
     *         pointer     : storage previously returned by allocate().
     *         objectCount : number of T objects the storage was sized for.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void deallocate(T *pointer, std::size_t objectCount) noexcept
    {
        bool fromArena = (reinterpret_cast<unsigned char *>(pointer) >= arena)
            && (reinterpret_cast<unsigned char *>(pointer) < arena + ARENA_BYTE_COUNT);
        std::cout << "  deallocate " << (objectCount * sizeof(T)) << " bytes ("
                  << (fromArena ? "arena, no-op" : "malloc, freeing") << ")\n";
        if (!fromArena)
        {
            std::free(pointer);
        }
    }

private:
    static inline unsigned char arena[ARENA_BYTE_COUNT] = {};
    static inline std::size_t arenaOffset = 0;
};

// Allocators of the same underlying arena type are interchangeable regardless of T.
template <typename T, typename U>
bool operator==(const LoggingBumpAllocator<T> &, const LoggingBumpAllocator<U> &)
{
    return true;
}

template <typename T, typename U>
bool operator!=(const LoggingBumpAllocator<T> &lhs, const LoggingBumpAllocator<U> &rhs)
{
    return !(lhs == rhs);
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Uses std::vector with LoggingBumpAllocator<int> to show the
 *         allocator being invoked for the container's reservation and
 *         growth reallocations instead of the default global allocator.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    std::vector<int, LoggingBumpAllocator<int>> values;

    std::cout << "-- reserve(4) --\n";
    values.reserve(4);

    std::cout << "-- push_back x5 (forces a reallocation past capacity 4) --\n";
    for (int i = 0; i < 5; i = i + 1)
    {
        values.push_back(i);
    }

    std::cout << "-- vector goes out of scope --\n";
    return 0;
}
