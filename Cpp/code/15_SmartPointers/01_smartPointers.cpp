#include <iostream>
#include <memory>
#include <string>

using namespace std;

// Demonstrates unique_ptr, shared_ptr, and weak_ptr.
class Resource
{
    private:
        string label;

    public:
        // Trivial constructor with initializer list.
        Resource(const string &resourceLabel) : label(resourceLabel)
        {
            cout << "Resource " << label << " created\n";
        }

        // Trivial destructor.
        ~Resource() { cout << "Resource " << label << " destroyed\n"; }

        // Trivial getter.
        const string &getLabel() const { return label; }
};

/*****************************************************************************
 * Name: inspectWeak
 *
 * Description:
 *         Attempts to lock a weak_ptr and prints whether the underlying
 *         Resource is still alive.
 *
 * Inputs:
 *         weakRef : a weak_ptr observing a Resource that may have expired.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void inspectWeak(const weak_ptr<Resource> &weakRef)
{
    shared_ptr<Resource> locked = weakRef.lock();
    if (locked)
    {
        cout << "weak_ptr still alive: " << locked->getLabel() << "\n";
    }
    else
    {
        cout << "weak_ptr has expired\n";
    }
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Exercises unique_ptr ownership, shared_ptr reference counting, and
 *         weak_ptr observation of a shared_ptr's lifetime.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    unique_ptr<Resource> unique = make_unique<Resource>("Unique");
    cout << "unique holds: " << unique->getLabel() << "\n";

    weak_ptr<Resource> weakRef;
    {
        shared_ptr<Resource> shared1 = make_shared<Resource>("Shared");
        shared_ptr<Resource> shared2 = shared1;
        weakRef = shared1;
        cout << "use_count = " << shared1.use_count() << "\n";
        inspectWeak(weakRef);
    }
    inspectWeak(weakRef);
    return 0;
}
