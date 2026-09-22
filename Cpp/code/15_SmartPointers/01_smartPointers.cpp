#include <iostream>
#include <memory>
#include <string>

// Demonstrates unique_ptr, shared_ptr, and weak_ptr.
class Resource
{
    private:
        std::string label;

    public:
        // Trivial constructor with initializer list.
        Resource(const std::string &resourceLabel) : label(resourceLabel)
        {
            std::cout << "Resource " << label << " created\n";
        }

        // Trivial destructor.
        ~Resource() { std::cout << "Resource " << label << " destroyed\n"; }

        // Trivial getter.
        const std::string &getLabel() const { return label; }
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
void inspectWeak(const std::weak_ptr<Resource> &weakRef)
{
    std::shared_ptr<Resource> locked = weakRef.lock();
    if (locked)
    {
        std::cout << "weak_ptr still alive: " << locked->getLabel() << "\n";
    }
    else
    {
        std::cout << "weak_ptr has expired\n";
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
    std::unique_ptr<Resource> unique = std::make_unique<Resource>("Unique");
    std::cout << "unique holds: " << unique->getLabel() << "\n";

    std::weak_ptr<Resource> weakRef;
    {
        std::shared_ptr<Resource> shared1 = std::make_shared<Resource>("Shared");
        std::shared_ptr<Resource> shared2 = shared1;
        weakRef = shared1;
        std::cout << "use_count = " << shared1.use_count() << "\n";
        inspectWeak(weakRef);
    }
    inspectWeak(weakRef);
    return 0;
}
