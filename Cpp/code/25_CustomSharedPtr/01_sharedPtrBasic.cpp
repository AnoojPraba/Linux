#include <iostream>
#include <atomic>
#include <functional>
#include <string>

#define DEFAULT_LABEL "unnamed"

// Hand-rolled std::shared_ptr, to see what the atomic refcounting under the
// real thing looks like. See 15_SmartPointers/01_smartPointers.cpp for the
// standard-library shared_ptr/weak_ptr usage this reimplements.

using namespace std;

/*****************************************************************************
 * Name: ControlBlock (struct)
 *
 * Description:
 *         Holds the managed raw pointer, an atomic strong reference count,
 *         and an optional custom deleter. One ControlBlock is shared by
 *         every SharedPtr that refers to the same managed object.
 *****************************************************************************/
template <typename T>
struct ControlBlock
{
    T *managedPointer;
    atomic<int> strongCount;
    function<void(T *)> deleter;

    ControlBlock(T *pointer, function<void(T *)> customDeleter)
        : managedPointer(pointer), strongCount(1), deleter(std::move(customDeleter))
    {
    }
};

template <typename T>
class SharedPtr
{
    private:
        ControlBlock<T> *control;

        /*****************************************************************************
         * Name: release
         *
         * Description:
         *         Decrements the strong reference count; if it reaches zero,
         *         destroys the managed object (via the deleter) and the
         *         control block itself.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void release()
        {
            if (!control)
            {
                return;
            }

            // The decrement-to-zero check needs memory_order_acq_rel (release
            // paired with an acquire fence on the thread that observes zero):
            // the release half publishes this thread's prior reads/writes of
            // the managed object so they happen-before destruction; the
            // acquire half ensures that if we're the thread that drove the
            // count to zero, we also see every other thread's uses of the
            // object that happened-before their own decrements. Using plain
            // relaxed here could let destruction run concurrently with
            // another thread still finishing up work through its own copy of
            // the pointer.
            if (control->strongCount.fetch_sub(1, memory_order_acq_rel) == 1)
            {
                if (control->deleter)
                {
                    control->deleter(control->managedPointer);
                }
                else
                {
                    delete control->managedPointer;
                }
                delete control;
            }
            control = nullptr;
        }

    public:
        /*****************************************************************************
         * Name: SharedPtr
         *
         * Description:
         *         Takes ownership of a raw pointer, allocating a fresh
         *         control block with a strong count of one.
         *
         * Inputs:
         *         pointer       : the raw pointer to take ownership of.
         *         customDeleter : optional deleter invoked instead of delete.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        explicit SharedPtr(T *pointer = nullptr, function<void(T *)> customDeleter = nullptr)
            : control(pointer ? new ControlBlock<T>(pointer, std::move(customDeleter)) : nullptr)
        {
        }

        // Copy constructor: shares the same control block, bumping the count.
        SharedPtr(const SharedPtr &other) : control(other.control)
        {
            if (control)
            {
                // The increment itself can be relaxed: the new reference is
                // only usable by this thread, and this thread already
                // synchronized-with the source SharedPtr's existence through
                // whatever handed it that copy (e.g. a mutex, or simply
                // being the same thread) -- there's no new cross-thread
                // visibility requirement being introduced by the increment
                // alone, only by the eventual decrement-to-zero.
                control->strongCount.fetch_add(1, memory_order_relaxed);
            }
        }

        // Copy assignment: release our own reference, then adopt other's.
        SharedPtr &operator=(const SharedPtr &other)
        {
            if (this == &other)
            {
                return *this;
            }
            release();
            control = other.control;
            if (control)
            {
                control->strongCount.fetch_add(1, memory_order_relaxed);
            }
            return *this;
        }

        // Move constructor: steal the pointer, no atomic traffic needed.
        SharedPtr(SharedPtr &&other) noexcept : control(other.control)
        {
            other.control = nullptr;
        }

        // Move assignment: release our own reference, then steal other's.
        SharedPtr &operator=(SharedPtr &&other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }
            release();
            control = other.control;
            other.control = nullptr;
            return *this;
        }

        // Destructor: releases our reference on scope exit.
        ~SharedPtr() { release(); }

        // Trivial dereference operators.
        T &operator*() const { return *control->managedPointer; }
        T *operator->() const { return control->managedPointer; }

        /*****************************************************************************
         * Name: useCount
         *
         * Description:
         *         Reports the current strong reference count.
         *
         * Returns:
         *         The strong reference count, or 0 if empty.
         *****************************************************************************/
        int useCount() const { return control ? control->strongCount.load(memory_order_relaxed) : 0; }

        // Trivial emptiness check.
        bool operator!() const { return control == nullptr; }
};

class NamedResource
{
    private:
        string label;

    public:
        // Trivial constructor with initializer list.
        explicit NamedResource(string resourceLabel = DEFAULT_LABEL)
            : label(std::move(resourceLabel))
        {
            cout << "NamedResource " << label << " created\n";
        }

        // Trivial destructor.
        ~NamedResource() { cout << "NamedResource " << label << " destroyed\n"; }

        // Trivial getter.
        const string &getLabel() const { return label; }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Exercises copy/move semantics and a custom deleter on SharedPtr.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    SharedPtr<NamedResource> first(new NamedResource("alpha"));
    cout << "useCount after construction = " << first.useCount() << "\n";

    SharedPtr<NamedResource> second = first;
    cout << "useCount after copy = " << first.useCount() << "\n";

    SharedPtr<NamedResource> third = std::move(second);
    cout << "useCount after move (unchanged) = " << first.useCount() << "\n";
    cout << "second is empty after move: " << (!second ? "true" : "false") << "\n";

    SharedPtr<NamedResource> withCustomDeleter(
        new NamedResource("beta"),
        [](NamedResource *pointer)
        {
            cout << "custom deleter invoked for " << pointer->getLabel() << "\n";
            delete pointer;
        });

    return 0;
}
