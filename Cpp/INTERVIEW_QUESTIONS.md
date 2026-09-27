# C++ Interview Question Bank

A companion to `code/` — not a duplicate of the topic `NOTES.md` files. This is a curated
set of the *kinds* of questions interviewers actually ask, organized by category, each
cross-referencing where in this repo to review the underlying material.

## 1. "What's wrong with this code?" snippets

- **A base class has virtual methods but a non-virtual destructor, and callers `delete`
  a derived object through a base pointer.** Only the base destructor runs, so the derived
  part (and anything it owns) leaks. Strong answer: any class meant to be used
  polymorphically needs a `virtual` destructor (or must forbid deletion via base pointer
  entirely). See `08_Inheritance`, `10_Polymorphism`.

  ```cpp
  // BUG: non-virtual destructor -- deleting through a Base* only runs ~Base().
  class Base
  {
      public:
          virtual void speak() { cout << "Base" << endl; }
          ~Base() { cout << "~Base" << endl; }
  };
  class Derived : public Base
  {
      public:
          ~Derived() { cout << "~Derived" << endl; }
  };
  Base *b = new Derived();
  delete b;
  // Output is just "~Base" -- ~Derived() never runs, leaking anything it owned.
  ```
  ```cpp
  // FIX: virtual destructor -- deleting through Base* now runs the full chain.
  class Base
  {
      public:
          virtual void speak() { cout << "Base" << endl; }
          virtual ~Base() { cout << "~Base" << endl; }
  };
  class Derived : public Base
  {
      public:
          ~Derived() { cout << "~Derived" << endl; }
  };
  Base *b = new Derived();
  delete b;
  // Output is "~Derived" then "~Base" -- both destructors run.
  ```
- **A class holds a raw pointer member, allocates it in the constructor, frees it in the
  destructor, but has no user-declared copy constructor/assignment.** The compiler
  generates a shallow copy, so two objects end up owning the same pointer -> double free
  or use-after-free. Fix: Rule of Three/Five, or just don't own a raw pointer (use
  `unique_ptr`/`shared_ptr`). See `04_ConstructorsAndDestructors`, `15_SmartPointers`.

  ```cpp
  // BUG: raw pointer member, no user-declared copy ctor/assignment -- the
  // compiler-generated copy is shallow, so both objects free the same pointer.
  class Buffer
  {
      private:
          int *data;

      public:
          explicit Buffer(int value) { data = new int(value); }
          ~Buffer() { delete data; }
  };
  Buffer a(1);
  Buffer b = a;
  // Both a and b now hold the same "data" pointer; when both go out of scope,
  // ~Buffer() runs twice on the same address: double free.
  ```
  ```cpp
  // FIX: own the resource via unique_ptr -- copying is disabled by default,
  // so the double-free can't happen; move transfers ownership instead.
  class Buffer
  {
      private:
          unique_ptr<int> data;

      public:
          explicit Buffer(int value) : data(make_unique<int>(value)) {}
  };
  Buffer a(1);
  Buffer b = std::move(a);
  // Buffer's copy ctor/assignment are implicitly deleted (unique_ptr member),
  // so "Buffer c = a;" would fail to compile instead of double-freeing.
  ```
- **A function returns a reference (or pointer) to a local variable.** The variable's
  storage is gone once the function returns, so the caller has a dangling reference —
  UB on first use. See `02_References`, `14_RAII`.

  ```cpp
  // BUG: returns a reference to a local -- its storage is gone on return.
  int &makeValue()
  {
      int local = 42;
      return local;
  }
  int &ref = makeValue();
  cout << ref << endl;
  // UB: ref refers to a destroyed stack frame; this may print 42, garbage,
  // or crash depending on what has since reused that stack memory.
  ```
  ```cpp
  // FIX: return by value -- the caller gets an independent copy (RVO/move
  // elides most of the cost anyway).
  int makeValue()
  {
      int local = 42;
      return local;
  }
  int value = makeValue();
  cout << value << endl;
  ```
- **Object slicing:** assigning a `Derived` object into a `Base` (by value, not by
  reference/pointer) copies only the `Base` subobject; the derived data and vtable are
  gone. Common in `std::vector<Base>` containers of "polymorphic" objects. See
  `10_Polymorphism`.
- **An exception is thrown out of a destructor** while another exception is already
  propagating (e.g. during stack unwinding). This calls `std::terminate`. Destructors
  should be `noexcept` (the implicit default) and swallow/log internally instead of
  throwing. See `13_ExceptionHandling`.
- **Comparing `std::optional<T>` to a raw `T` value without checking `has_value()`
  first**, or writing `if (opt == 0)` when `opt` is empty and expecting false — an empty
  optional compares unequal to every value, which is easy to get backwards in guard
  logic (e.g. `if (!opt || *opt != x)` vs. accidentally dereferencing an empty optional).
  See `20_ModernCppFeatures`.
- **A lambda captures a local variable by reference (`[&]`) and is stored/returned for
  later use** (e.g. as a `std::function` outlives the enclosing scope, or queued onto
  another thread). The captured reference dangles once the local goes out of scope.
  Fix: capture by value, or capture a `shared_ptr` for shared ownership. See
  `16_Lambdas`, `24_Concurrency`.
- **A class with a `shared_ptr` member forms a reference cycle with another object that
  also holds a `shared_ptr` back to it.** Neither strong count ever reaches zero, so both
  leak even though "smart pointers" are being used correctly. See
  `25_CustomSharedPtr/NOTES.md` (WeakPtr section), `15_SmartPointers`.

## 2. Design / tradeoff questions

- **`unique_ptr` vs. `shared_ptr` vs. raw pointer/reference — when does each apply?**
  Strong answer: `unique_ptr` for sole ownership (default choice), `shared_ptr` only when
  ownership is genuinely shared and lifetime is unclear at compile time, raw
  pointer/reference for non-owning observation where the referent's lifetime is
  guaranteed by someone else. See `15_SmartPointers`, `25_CustomSharedPtr`.

  ```cpp
  void withRawPointer()
  {
      Widget *w = new Widget();
      w->ping();
      // Every exit path (including an exception between new and delete) must
      // remember this delete, or it leaks.
      delete w;
  }
  void withUniquePtr()
  {
      unique_ptr<Widget> w = make_unique<Widget>();
      w->ping();
      // Freed automatically at scope exit, even if ping() throws -- no
      // matching delete to forget.
  }
  ```
- **When would you prefer composition over inheritance?** Inheritance couples derived
  classes to base implementation details and is fixed at compile time; composition
  is more flexible (can swap the contained object at runtime) and avoids fragile-base-class
  problems. Rule of thumb: prefer composition unless there's a genuine "is-a" relationship
  with shared behavior via virtual dispatch. See `08_Inheritance`, `31_SOLIDPrinciples`.
- **When is CRTP (static polymorphism) preferable to virtual functions, and when isn't
  it?** CRTP avoids vtable indirection and enables inlining — good for performance-critical
  code with a closed, compile-time-known set of types. Virtual dispatch wins when the set
  of types is open/runtime-determined (plugins, polymorphic containers) since CRTP can't
  give you a single container of "any derived type." See `26_TemplateMetaprogramming`,
  `10_Polymorphism`.

  ```cpp
  // Static dispatch (CRTP): resolved at compile time, no vtable indirection.
  template <typename Derived>
  class ShapeCRTP
  {
      public:
          void draw() { static_cast<Derived *>(this)->drawImpl(); }
  };
  class CircleCRTP : public ShapeCRTP<CircleCRTP>
  {
      public:
          void drawImpl() { cout << "CircleCRTP" << endl; }
  };

  // Dynamic dispatch: resolved at runtime via vtable, supports open sets of
  // types in one container (e.g. vector<ShapeVirtual *>).
  class ShapeVirtual
  {
      public:
          virtual void draw() { cout << "ShapeVirtual" << endl; }
          virtual ~ShapeVirtual() {}
  };
  class CircleVirtual : public ShapeVirtual
  {
      public:
          void draw() override { cout << "CircleVirtual" << endl; }
  };

  CircleCRTP crtp;
  crtp.draw();                                 // -> "CircleCRTP", no vtable lookup
  ShapeVirtual *virt = new CircleVirtual();
  virt->draw();                                // -> "CircleVirtual", via vtable
  delete virt;
  ```
- **`std::variant` + visitation vs. a class hierarchy with virtual dispatch — when would
  you pick each?** `variant` suits a closed, rarely-changing set of alternatives with
  value semantics and no heap allocation; a hierarchy suits an open set of types where
  new kinds are added independently (adding a variant alternative touches every
  visitor, adding a subclass doesn't touch existing callers). See `20_ModernCppFeatures`,
  `10_Polymorphism`.
- **Why might you choose `std::vector` over `std::list` even with frequent
  insertions/removals?** Cache locality: contiguous storage means far fewer cache misses
  during iteration, which usually dominates the asymptotic cost of `list`'s O(1) insert —
  `vector`'s amortized O(1) push_back and cheaper iteration often win in practice unless
  insertions are frequent in the *middle* of a large container. See `18_STLContainers`,
  `27_STLInternals`.
- **Why does `make_shared` matter over `shared_ptr<T>(new T(...))`?** Single combined
  allocation for object + control block (fewer allocations, better locality, exception
  safety against a leaked raw pointer if the control-block allocation throws). See
  `25_CustomSharedPtr/NOTES.md`.
- **When would you reach for a custom allocator instead of the default heap
  allocator?** Predictable/bounded latency (real-time paths), reducing fragmentation for
  many same-size objects (pool allocators), or arena allocation for objects with a shared
  lifetime that can all be freed at once. See `29_CustomAllocatorCpp`.

## 3. Concurrency questions

- **`std::mutex` vs. `std::atomic` — when do you use each?** `atomic` suits a single
  variable with simple read/modify/write operations (counters, flags) with no need to
  keep multiple pieces of state consistent together; `mutex` is needed once more than one
  piece of state must change together atomically, or the critical section does anything
  beyond a single atomic op. See `24_Concurrency`.
- **What's a data race, and how does it differ from a race condition in general?** A data
  race is a specific, always-undefined-behavior case: two threads access the same memory
  concurrently, at least one is a write, with no synchronization. A race condition is the
  broader concept of correctness depending on timing/interleaving — it may or may not
  involve a data race (e.g. two threads race to acquire a properly-locked mutex, which is
  a race condition but not a data race). See `24_Concurrency`.
- **Why is defaulting to `std::memory_order_relaxed` dangerous?** It only guarantees
  atomicity of the operation itself, not ordering/visibility of other memory operations
  around it — using it where acquire/release semantics are actually needed (e.g.
  publishing a pointer then a flag) can let another thread observe the flag before the
  data it guards, a subtle bug that won't show up in testing on most platforms. See
  `24_Concurrency`, `25_CustomSharedPtr/NOTES.md`.
- **How do you design a thread-safe singleton in modern C++ without a mutex?** A
  function-local `static` is guaranteed by the standard (since C++11) to be initialized
  exactly once even under concurrent first calls ("magic statics") — the compiler emits
  the synchronization for you, so no explicit locking or double-checked locking pattern
  is needed. See `24_Concurrency`, `DesignPatterns/Creational/Singleton.cpp`.

  ```cpp
  class Logger
  {
      public:
          static Logger &instance()
          {
              // Since C++11, initialization of a function-local static is
              // guaranteed thread-safe: concurrent first callers block until
              // the one-time construction finishes, no explicit mutex needed.
              static Logger single;
              return single;
          }

      private:
          Logger() {}
  };
  // Launching THREAD_COUNT threads that each call Logger::instance() prints
  // the same "this" address every time -- exactly one instance was built.
  ```
- **What's false sharing, and how do you avoid it?** Two threads write to unrelated
  variables that happen to sit on the same cache line, causing needless cache-line
  ping-pong between cores; fix by padding/aligning hot variables to separate cache lines.
  Relevant when discussing the lock-striped LRU cache design. See `24_Concurrency`.

## 4. Modern C++ / "why did the language add this" questions

- **Why does C++11 have both `std::move` and `std::forward` if they seem similar?**
  `std::move` unconditionally casts to an rvalue reference (says "I'm done with this");
  `std::forward` conditionally preserves the value category of a forwarding-reference
  argument so a generic wrapper can pass it through as it was passed in ("perfect
  forwarding"). Using `move` inside a template where `forward` is needed breaks lvalue
  callers. See `21_MoveSemantics`.

  ```cpp
  // std::move: unconditionally treats "value" as an rvalue -- correct here
  // because this function owns "value" and is done with it.
  void takeOwnership(string value)
  {
      consume(std::move(value));
  }

  // std::forward: preserves whatever value category the caller actually used
  // for a forwarding reference -- move would wrongly force-move an lvalue too.
  template <typename T>
  void relay(T &&arg)
  {
      consume(std::forward<T>(arg));
  }
  string lvalue = "lvalue";
  relay(lvalue);              // forwarded as an lvalue -> copy path
  relay(string("temporary")); // forwarded as an rvalue -> moved-from path
  ```
- **What problem does `unique_ptr` solve that raw pointers + manual `delete` didn't?**
  Deterministic, exception-safe cleanup tied to scope (RAII) — no forgotten `delete`, no
  leak if an exception is thrown between allocation and the matching `delete`, and clear
  ownership semantics expressed in the type itself. See `14_RAII`, `15_SmartPointers`.
- **Why mark a function `noexcept`, and what happens if it throws anyway?** It documents
  a guarantee callers (and the compiler) can rely on — e.g. enabling `vector` to move
  instead of copy elements on reallocation. If a `noexcept` function throws, the runtime
  calls `std::terminate` immediately rather than unwinding. See `23_NoexceptAndSTL`.

  ```cpp
  // Promises never to throw; if it does, std::terminate is called immediately
  // -- there is no unwinding, no catch block anywhere can intercept this.
  void mustNotThrow() noexcept
  {
      throw runtime_error("broke the promise");
  }
  mustNotThrow();
  // Never reached: the process aborts via std::terminate before returning.
  ```
- **SFINAE vs. C++20 concepts — why did concepts get added when SFINAE already
  worked?** SFINAE achieves constrained templates via convoluted trait/`void_t` tricks in
  the type system, producing unreadable error messages when a constraint fails. Concepts
  express the same intent as a named, readable constraint checked up front, with
  far clearer compiler diagnostics. See `26_TemplateMetaprogramming`, `30_Cpp20Features`.
- **Why did C++17 add `std::optional` instead of just using a pointer or a sentinel
  value to mean "no value"?** Expresses "may or may not have a value" in the type itself
  with value semantics (no heap allocation, no ownership ambiguity like a pointer implies),
  and avoids magic sentinel values that collide with valid data. See `20_ModernCppFeatures`.
- **What does copy elision / RVO guarantee you in C++17 that C++11 only allowed as an
  optimization?** C++17 makes elision of the temporary in `return T(...);`-style
  construction mandatory (guaranteed, not just permitted), meaning no move/copy
  constructor is even required to be present for that case. See `22_CopyElisionAndRVO`.

## 5. Explain-it-simply questions

- **Explain RAII to someone who's never heard the term.** Tie a resource's lifetime to an
  object's scope: acquire the resource in the constructor, release it in the destructor,
  so it's automatically cleaned up when the object goes out of scope — even if an
  exception is thrown in between. See `14_RAII`.

  ```cpp
  class FileHandleRAII
  {
      private:
          string name;

      public:
          explicit FileHandleRAII(const string &fileName) : name(fileName)
          {
              cout << "acquire " << name << endl;
          }
          ~FileHandleRAII()
          {
              cout << "release " << name << endl;
          }
  };
  void process()
  {
      FileHandleRAII handle("log.txt");
      throw runtime_error("something failed mid-function");
      // "release log.txt" still prints -- the destructor runs during stack
      // unwinding, even though we never reached the end of process().
  }
  ```
- **Explain what a vtable is and why virtual calls have overhead.** Each polymorphic
  object carries a hidden pointer to a per-class table of function pointers; a virtual
  call is an extra pointer dereference through that table rather than a direct call,
  which also usually defeats inlining. See `10_Polymorphism`.
- **Explain the Rule of Three/Five/Zero and why it exists.** If you write one of
  destructor/copy-constructor/copy-assignment, you probably need all three (managing a
  resource); C++11 added move constructor/assignment as the Rule of Five. Rule of Zero:
  best of all is to own no raw resources directly and let member smart
  pointers/containers handle it, so you need to write none of the five. See
  `04_ConstructorsAndDestructors`, `15_SmartPointers`.
- **Explain what a memory leak still looks like in C++ despite having smart pointers.**
  Two `shared_ptr`s holding each other in a cycle never reach a zero refcount; or a raw
  resource (file handle, socket, mutex) is acquired without any RAII wrapper at all and
  a code path skips the matching manual release. Smart pointers prevent the "forgot to
  free" case, not the "designed the ownership graph wrong" case. See
  `25_CustomSharedPtr/NOTES.md`, `14_RAII`.
- **Explain move semantics in plain terms.** Instead of deep-copying a resource
  (allocating new memory and copying every byte), a move steals the internal pointers
  from a source object that's about to be destroyed/discarded anyway, leaving it in a
  valid-but-unspecified empty state. See `21_MoveSemantics`.
- **Explain what "the Liskov Substitution Principle" means with a concrete example.**
  A subtype must be usable wherever its base type is expected without surprising callers
  — the classic violation is a `Square` inheriting from `Rectangle` that overrides
  `setWidth`/`setHeight` to keep both sides equal, breaking code that assumes setting one
  doesn't affect the other. See `31_SOLIDPrinciples`.

## 6. Design pattern questions

- **When would you use Strategy instead of an if/else or switch chain picking
  behavior?** Strategy moves each behavior into its own class behind a common interface,
  so adding a new behavior means adding a class instead of editing a growing conditional
  — useful when the set of behaviors changes independently or is chosen at runtime.
  See `DesignPatterns/Behavioral/Strategy.cpp`.

  ```cpp
  // Before: adding a new kind means editing this function again.
  double priceIfElse(double amount, DiscountKind kind)
  {
      if (kind == DiscountKind::NONE)
      {
          return amount;
      }
      else if (kind == DiscountKind::PERCENT)
      {
          return amount * 0.9;
      }
      return amount;
  }

  // After (Strategy): adding a new kind means adding a class, not editing
  // existing ones. Full pattern lives in DesignPatterns/Behavioral/Strategy.cpp.
  class DiscountStrategy
  {
      public:
          virtual double apply(double amount) = 0;
          virtual ~DiscountStrategy() {}
  };
  class PercentDiscount : public DiscountStrategy
  {
      public:
          double apply(double amount) override { return amount * 0.9; }
  };
  ```
- **What's the difference between Factory Method and Abstract Factory?** Factory Method
  is a single virtual creation method overridden per subclass to produce one product;
  Abstract Factory is an interface for creating a *family* of related products that must
  stay consistent with each other (e.g. matching UI widgets for one theme). See
  `DesignPatterns/Creational/FactoryMethod.cpp`,
  `DesignPatterns/Creational/AbstractFactory.cpp`.
- **Why is the classic Singleton pattern often criticized, and what are the
  alternatives?** It's effectively global mutable state — hard to unit test (can't swap
  in a fake), hides dependencies from constructors, and can hide lifetime/ordering bugs
  across translation units. Alternatives: dependency injection of a single shared
  instance, or making global-ness explicit and minimal. See
  `DesignPatterns/Creational/Singleton.cpp`, `24_Concurrency` (magic statics).

  ```cpp
  // Alternative to a global Singleton: inject one shared instance explicitly.
  // Dependencies are visible in the constructor and a fake is swappable in tests.
  class Report
  {
      private:
          shared_ptr<ConfigService> config;

      public:
          explicit Report(shared_ptr<ConfigService> configService)
              : config(std::move(configService))
          {
          }
          void build() { config->read(); }
  };
  shared_ptr<ConfigService> config = make_shared<ConfigService>();
  Report report(config);
  report.build();
  ```
- **Explain the Observer pattern and where you've seen it in a real system.** Subjects
  notify a list of registered observers on state change without knowing their concrete
  types — the classic examples are GUI event handling and pub/sub messaging systems.
  Watch for: dangling observer pointers if an observer is destroyed without
  unsubscribing. See `DesignPatterns/Behavioral/Observer.cpp`.
- **When would you use Decorator instead of subclassing to add behavior?** Decorator
  wraps an object to add behavior at runtime, combinably, without an explosion of
  subclasses for every combination of add-ons (e.g. stacking stream encoders). See
  `DesignPatterns/Structural/Decorator.cpp`.
- **What problem does the Adapter pattern solve, and how is it different from
  Facade?** Adapter makes one existing interface look like another interface a client
  already expects (translating, one-to-one); Facade provides a new, simplified interface
  over a whole subsystem of many classes (simplifying, one-to-many). See
  `DesignPatterns/Structural/Adapter.cpp`, `DesignPatterns/Structural/Facade.cpp`.
- **Explain the State pattern and how it differs from just using an enum + switch.**
  State encapsulates each state's behavior and transitions in its own class implementing
  a common interface, so the object's behavior changes as its internal state object
  changes — avoids one large switch statement that must be touched for every new state
  and keeps each state's logic isolated. See `DesignPatterns/Behavioral/State.cpp`.
