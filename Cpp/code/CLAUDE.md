# Cpp/code

Index of the 34 numbered C++ topic folders plus `DesignPatterns/`. Each folder has its own CLAUDE.md with files, build line, concepts, gotchas and cross-references.

## Build everything
- `make` here builds every `.cpp` (one binary per file, found recursively) into `bin/<NN_Topic>/<name>` (`Cpp/code/bin/`, git-ignored), in parallel (`MAKEFLAGS += -j$(nproc)`). `make clean` removes `bin/`.
- Flags: `g++ -std=c++17 -Wall -I. -pthread`. Exception: `33_Cpp20Coroutines` and `34_SpanStringViewAndPmr` build with `-std=c++20` (per-folder rule in the Makefile; needs GCC >= 10).
- Single file by hand: `g++ -std=c++17 -Wall -Wextra -pthread NN_Topic/file.cpp -o /tmp/x && /tmp/x` (`-std=c++20` for 33 and 34).
- No multi-file or extra-library rules: each `.cpp` is a standalone program, so demos of cross-TU issues (`05_StaticMembers`, `28_PImplIdiom`) are single-file simplifications.

## Numbering and layout
- Two-digit prefix gives the study order, contiguous from `01` to `34`. Files inside a folder carry `NN_` prefixes (a few collide, e.g. two `07_` and two `08_` files in `24_Concurrency`; `26_TemplateMetaprogramming` has no `03_`).
- `DesignPatterns/` has no numeric prefix: `Creational/`, `Structural/`, `Behavioral/`, one CLAUDE.md per group, one `.cpp` per pattern.
- `33`, `34` end NOTES.md with a "Senior interviewer Q&A" section; `30_Cpp20Features` holds a C++17 fallback and a (stale) toolchain note.
- Deliberately flawed demos: `10_Polymorphism/02_virtualDestructor.cpp` (UB delete), `24_Concurrency/10_deadlockAndScopedLock.cpp` (deadlock path never called).
- `../INTERVIEW_QUESTIONS.md` is the question bank for this domain (one level up from this folder).

## Folder map
| Folder | Purpose |
|---|---|
| `01_Namespaces` | Namespace basics: nested namespaces, qualification and using declarations |
| `02_References` | Lvalue references, pointers vs references |
| `03_ClassesAndObjects` | Class basics: access specifiers, member functions and the mutable keyword (logical vs bitwise constness) |
| `04_ConstructorsAndDestructors` | Special member functions: rule of three, copy-and-swap, rule of zero and explicit |
| `05_StaticMembers` | Static data/member functions and the static initialization order fiasco with its construct-on-first-use fix |
| `06_FriendFunctionsAndClasses` | friend functions and friend classes granting access to private members |
| `07_OperatorOverloading` | Operator overloading on a 2D vector: +, ==, << |
| `08_Inheritance` | Single inheritance, constructor chaining, protected members and object slicing |
| `09_MultipleInheritanceAndVirtualBase` | The diamond problem in multiple inheritance and its fix with virtual inheritance |
| `10_Polymorphism` | Virtual functions, override, abstract classes |
| `11_TypeCasting` | The four named C++ casts and when each is appropriate |
| `12_StreamsAndIO` | iostream file and string streams: ofstream, ifstream, stringstream parsing |
| `13_ExceptionHandling` | Exceptions: custom exception classes, throw/catch order and what() overrides |
| `14_RAII` | Resource Acquisition Is Initialisation: a FILE* guard whose destructor always closes the file |
| `15_SmartPointers` | Standard smart pointers: unique_ptr (exclusive), shared_ptr (shared count) and weak_ptr (non-owning observer) |
| `16_Lambdas` | Lambda syntax, capture by value/reference |
| `17_Templates` | Function and class templates: generic code resolved at compile time |
| `18_STLContainers` | STL containers and adaptors with complexity and behaviour trade-offs, including unordered containers and small string... |
| `19_STLAlgorithms` | Core <algorithm>/<numeric> usage on a vector: sort, find, transform, accumulate |
| `20_ModernCppFeatures` | C++11-C++20 language and library features every modern codebase uses: auto/decltype, structured bindings, constexpr,... |
| `21_MoveSemantics` | Move constructor and move assignment on a class owning a heap array |
| `22_CopyElisionAndRVO` | RVO/NRVO and C++17 guaranteed copy elision, observed with a class that logs every special member call |
| `23_NoexceptAndSTL` | Why noexcept move operations matter: std::vector falls back to copying on reallocation if move can throw |
| `24_Concurrency` | C++ standard-library concurrency from threads and mutexes to condition variables, atomics with memory orderings,... |
| `25_CustomSharedPtr` | Hand-rolled shared_ptr and weak_ptr with an atomic two-count control block, to explain what std::shared_ptr does |
| `26_TemplateMetaprogramming` | Compile-time programming: variadic templates and fold expressions, SFINAE, CRTP, type traits, type erasure and... |
| `27_STLInternals` | How STL containers behave internally: vector growth, iterator invalidation, map vs unordered_map and writing a custom... |
| `28_PImplIdiom` | Pointer-to-implementation idiom for compile-time firewalling and ABI stability |
| `29_CustomAllocatorCpp` | A minimal std-conforming bump/arena allocator that logs every allocation made by std::vector |
| `30_Cpp20Features` | C++20 concepts and ranges |
| `31_SOLIDPrinciples` | The five SOLID principles, each as a violation followed by a refactor |
| `32_RpcMechanismsCpp` | RPC mechanics in modern C++: a type-erased dispatch table and a future-based async client (simulated, no real network) |
| `33_Cpp20Coroutines` | C++20 coroutines from first principles: a generator with co_yield and a cooperative round-robin scheduler |
| `34_SpanStringViewAndPmr` | std::span, std::string_view and std::pmr arena allocation: non-owning views and allocation control |
| `DesignPatterns/Creational` | Abstract Factory, Builder, Factory Method, Prototype, Singleton |
| `DesignPatterns/Structural` | Adapter, Composite, Decorator, Facade, Proxy |
| `DesignPatterns/Behavioral` | Chain of Responsibility, Command, Iterator, Mediator, Memento, Observer, State, Strategy, Template Method, Visitor |

## Cross-domain links
- C equivalents and building blocks: `../../C_Basics/code` (e.g. `55_VTableEmulation`, `54_OpaquePointer`, `82_CoroutinesInC`).
- OS-level counterparts: `../../OS/code` (threads, atomics, allocators, RPC, epoll).
