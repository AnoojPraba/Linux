#include <cstddef>
#include <iostream>
#include <memory_resource>
#include <string>
#include <vector>

// std::pmr (polymorphic memory resources, C++17): containers take a runtime
// `memory_resource*` instead of a compile-time Allocator template parameter, so
// vector<int> with a stack arena has the SAME TYPE as one using the heap.
// Combine with monotonic_buffer_resource for fast, malloc-free, per-request
// arenas: allocation is a pointer bump, deallocation is a no-op, everything is
// released at once when the resource dies.

// Wrap the default resource to COUNT how often the real heap is hit.
class CountingResource : public std::pmr::memory_resource
{
public:
    explicit CountingResource(std::pmr::memory_resource *up) : up_(up) {}
    int allocs = 0;
    std::size_t bytes = 0;

private:
    void *do_allocate(std::size_t n, std::size_t a) override
    {
        ++allocs;
        bytes += n;
        return up_->allocate(n, a);
    }
    void do_deallocate(void *p, std::size_t n, std::size_t a) override { up_->deallocate(p, n, a); }
    bool do_is_equal(const std::pmr::memory_resource &o) const noexcept override { return this == &o; }
    std::pmr::memory_resource *up_;
};

static void fill(std::pmr::vector<std::pmr::string> &names)
{
    for (int i = 0; i < 20; ++i)
        names.emplace_back("a-fairly-long-name-that-defeats-small-string-optimization-" + std::to_string(i));
}

int main()
{
    // 1) Plain heap: every string and every vector growth hits new/delete.
    {
        CountingResource heap(std::pmr::new_delete_resource());
        std::pmr::vector<std::pmr::string> names(&heap);
        fill(names);
        std::cout << "heap resource : " << heap.allocs << " upstream allocations, "
                  << heap.bytes << " bytes\n";
    }

    // 2) Stack buffer arena: the upstream (heap) is never touched while it fits.
    {
        alignas(std::max_align_t) std::byte buffer[16 * 1024];
        CountingResource upstream(std::pmr::new_delete_resource());
        std::pmr::monotonic_buffer_resource arena(buffer, sizeof buffer, &upstream);
        std::pmr::vector<std::pmr::string> names(&arena);
        fill(names);
        std::cout << "stack arena   : " << upstream.allocs << " upstream allocations "
                  << "(arena served the rest from the stack)\n";
    }

    // 3) Overflowing the buffer falls back to the upstream in growing chunks.
    {
        std::byte tiny[256];
        CountingResource upstream(std::pmr::new_delete_resource());
        std::pmr::monotonic_buffer_resource arena(tiny, sizeof tiny, &upstream);
        std::pmr::vector<std::pmr::string> names(&arena);
        fill(names);
        std::cout << "tiny arena    : " << upstream.allocs << " upstream allocations (buffer exhausted)\n";
    }

    // Pitfalls: monotonic resources NEVER reuse freed memory (a long-lived
    // vector that grows repeatedly wastes the old buffers until the arena is
    // released); objects must not outlive the arena; containers propagate their
    // allocator to elements only when the element type is also pmr-aware.
}
