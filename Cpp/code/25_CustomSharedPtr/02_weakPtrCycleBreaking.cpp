#include <iostream>
#include <atomic>

// Two-count control block: strong count reaching zero destroys the managed
// object; strong AND weak both reaching zero destroys the control block
// itself. This split is what lets a WeakPtr safely outlive the object it
// observes (it can still tell "the object is gone") without keeping the
// object itself alive, and without the control block being freed out from
// under a WeakPtr that's still checking it.

using namespace std;

template <typename T>
struct ControlBlock
{
    T *managedPointer;
    atomic<int> strongCount;
    atomic<int> weakCount;

    ControlBlock(T *pointer) : managedPointer(pointer), strongCount(1), weakCount(0) {}
};

template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr
{
    private:
        ControlBlock<T> *control;

        friend class WeakPtr<T>;

        // Private constructor used only by WeakPtr::lock() to build a
        // SharedPtr from a control block whose strong count was already
        // incremented by the caller.
        explicit SharedPtr(ControlBlock<T> *existingControl) : control(existingControl) {}

        void releaseStrong()
        {
            if (!control)
            {
                return;
            }
            if (control->strongCount.fetch_sub(1, memory_order_acq_rel) == 1)
            {
                delete control->managedPointer;
                control->managedPointer = nullptr;
                if (control->weakCount.load(memory_order_acquire) == 0)
                {
                    delete control;
                }
            }
            control = nullptr;
        }

    public:
        // Trivial default constructor: an empty SharedPtr.
        SharedPtr() : control(nullptr) {}

        explicit SharedPtr(T *pointer) : control(pointer ? new ControlBlock<T>(pointer) : nullptr) {}

        SharedPtr(const SharedPtr &other) : control(other.control)
        {
            if (control)
            {
                control->strongCount.fetch_add(1, memory_order_relaxed);
            }
        }

        SharedPtr &operator=(const SharedPtr &other)
        {
            if (this == &other)
            {
                return *this;
            }
            releaseStrong();
            control = other.control;
            if (control)
            {
                control->strongCount.fetch_add(1, memory_order_relaxed);
            }
            return *this;
        }

        SharedPtr(SharedPtr &&other) noexcept : control(other.control) { other.control = nullptr; }

        SharedPtr &operator=(SharedPtr &&other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }
            releaseStrong();
            control = other.control;
            other.control = nullptr;
            return *this;
        }

        ~SharedPtr() { releaseStrong(); }

        T &operator*() const { return *control->managedPointer; }
        T *operator->() const { return control->managedPointer; }
        bool operator!() const { return (!control) || (!control->managedPointer); }
};

template <typename T>
class WeakPtr
{
    private:
        ControlBlock<T> *control;

    public:
        // Trivial default constructor: an empty WeakPtr.
        WeakPtr() : control(nullptr) {}

        /*****************************************************************************
         * Name: WeakPtr
         *
         * Description:
         *         Observes the same control block as a SharedPtr, incrementing
         *         only the weak count.
         *
         * Inputs:
         *         source : the SharedPtr whose control block to observe.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        WeakPtr(const SharedPtr<T> &source) : control(source.control)
        {
            if (control)
            {
                control->weakCount.fetch_add(1, memory_order_relaxed);
            }
        }

        WeakPtr(const WeakPtr &other) : control(other.control)
        {
            if (control)
            {
                control->weakCount.fetch_add(1, memory_order_relaxed);
            }
        }

        WeakPtr &operator=(const WeakPtr &other)
        {
            if (this != &other)
            {
                reset();
                control = other.control;
                if (control)
                {
                    control->weakCount.fetch_add(1, memory_order_relaxed);
                }
            }
            return *this;
        }

        /*****************************************************************************
         * Name: reset
         *
         * Description:
         *         Drops this WeakPtr's observation, decrementing the weak
         *         count and freeing the control block if both counts have
         *         reached zero.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void reset()
        {
            if (!control)
            {
                return;
            }
            if ((control->weakCount.fetch_sub(1, memory_order_acq_rel) == 1) &&
                (control->strongCount.load(memory_order_acquire) == 0))
            {
                delete control;
            }
            control = nullptr;
        }

        ~WeakPtr() { reset(); }

        /*****************************************************************************
         * Name: lock
         *
         * Description:
         *         Attempts to obtain a SharedPtr to the observed object,
         *         succeeding only if the object hasn't already been destroyed.
         *
         * Returns:
         *         A valid SharedPtr<T> if the object is still alive, or an
         *         empty SharedPtr<T> if it has already been destroyed.
         *****************************************************************************/
        SharedPtr<T> lock() const
        {
            if (!control)
            {
                return SharedPtr<T>();
            }

            int currentStrong = control->strongCount.load(memory_order_relaxed);
            while (currentStrong > 0)
            {
                if (control->strongCount.compare_exchange_weak(
                        currentStrong, currentStrong + 1, memory_order_acq_rel))
                {
                    return SharedPtr<T>(control);
                }
            }
            return SharedPtr<T>();
        }
};

class Parent;

class Child
{
    private:
        // Observes the parent without extending its lifetime; using
        // SharedPtr<Parent> here instead would form parent -> child -> parent
        // reference cycle, so neither object's strong count would ever reach
        // zero and both would leak. No broken cycle version is implemented
        // here on purpose -- see NOTES.md.
        WeakPtr<Parent> parentRef;

    public:
        // Trivial constructor.
        Child() { cout << "Child created\n"; }

        // Trivial destructor.
        ~Child() { cout << "Child destroyed\n"; }

        /*****************************************************************************
         * Name: setParent
         *
         * Description:
         *         Records a non-owning observation of the parent.
         *
         * Inputs:
         *         parent : SharedPtr to the parent to observe.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void setParent(const SharedPtr<Parent> &parent) { parentRef = parent; }

        /*****************************************************************************
         * Name: describeParentStatus
         *
         * Description:
         *         Prints whether the observed parent is still alive.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void describeParentStatus()
        {
            SharedPtr<Parent> lockedParent = parentRef.lock();
            if (!lockedParent)
            {
                cout << "Child: parent is gone\n";
            }
            else
            {
                cout << "Child: parent is still alive\n";
            }
        }
};

class Parent
{
    private:
        SharedPtr<Child> child;

    public:
        // Trivial constructor.
        Parent() : child(new Child()) { cout << "Parent created\n"; }

        // Trivial destructor.
        ~Parent() { cout << "Parent destroyed\n"; }

        // Trivial getter.
        Child &getChild() { return *child; }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Builds a Parent owning a Child that weakly observes the Parent,
 *         then demonstrates WeakPtr::lock() before and after the Parent is
 *         destroyed.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    WeakPtr<Parent> observedParent;

    {
        SharedPtr<Parent> parent(new Parent());
        parent->getChild().setParent(parent);
        observedParent = WeakPtr<Parent>(parent);

        parent->getChild().describeParentStatus();
    }

    SharedPtr<Parent> lockedAfterDestruction = observedParent.lock();
    cout << "lock() after parent destroyed is empty: "
               << (!lockedAfterDestruction ? "true" : "false") << "\n";

    return 0;
}
