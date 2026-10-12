// Where globals, static members, function-local statics, thread_local, heap,
// vector elements, locals and polymorphic objects live.

#include <iostream>
#include <memory>
#include <string>
#include <vector>

extern "C" char etext, edata, end;           // boundary symbols supplied by the GNU linker

int initialized = 10;                        // data
int uninitialized;                           // bss
const char *literal = "text";                // pointer in data, characters in read-only data
thread_local int per_thread = 3;             // thread-local storage (TLS), one copy per thread

struct Base
{
    static int count;                        // static member: bss
    virtual ~Base() = default;
    virtual int f() const { return 1; }
};
int Base::count;

const char *where(const void *p)
{
    const char *a = static_cast<const char *>(p);
    if (a < &etext) return "code";
    if (a < &edata) return "data";
    if (a < &end)   return "bss";
    return "heap, stack or mapped memory";
}

int main()
{
    int local = 1;
    static int counter;
    auto heap = std::make_unique<int>(7);
    std::vector<int> v(1000);
    Base obj;

    std::cout << "function main        : " << where(reinterpret_cast<const void *>(&main)) << '\n';
    std::cout << "initialized global   : " << where(&initialized) << '\n';
    std::cout << "uninitialized global : " << where(&uninitialized) << '\n';
    std::cout << "static data member   : " << where(&Base::count) << '\n';
    std::cout << "function-local static: " << where(&counter) << '\n';
    std::cout << "string literal       : " << where(literal) << '\n';
    std::cout << "thread_local         : " << (where(&per_thread)[0] == 'h' ? "outside the main image (TLS block)" : "in the image") << '\n';
    std::cout << "make_unique<int>     : " << where(heap.get()) << '\n';
    std::cout << "vector elements      : " << where(v.data()) << '\n';
    std::cout << "local variable       : " << where(&local) << '\n';
    std::cout << "polymorphic object   : " << where(&obj) << '\n';
}
