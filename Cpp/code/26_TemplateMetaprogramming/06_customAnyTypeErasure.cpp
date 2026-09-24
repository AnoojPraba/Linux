#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>
#include <stdexcept>

// Demonstrates hand-rolled type erasure -- the technique std::any is built on.
// A MyAny can hold a value of any copyable type behind one non-template
// class, by hiding the concrete type behind a small abstract interface
// (Concept) and a templated implementation of that interface (Model<T>).

using namespace std;

class MyAny
{
    private:
        struct Concept
        {
            virtual ~Concept() = default;
            virtual unique_ptr<Concept> clone() const = 0;
            virtual const type_info &type() const = 0;
        };

        template <typename T>
        struct Model : Concept
        {
            T value;

            explicit Model(T storedValue) : value(std::move(storedValue)) {}

            unique_ptr<Concept> clone() const override
            {
                return make_unique<Model<T>>(value);
            }

            const type_info &type() const override { return typeid(T); }
        };

        unique_ptr<Concept> storage;

    public:
        // Trivial default constructor: an empty MyAny holds nothing.
        MyAny() = default;

        // Templated constructor: wraps any copyable T in a Model<T>, erasing
        // its concrete type behind the Concept interface.
        template <typename T>
        MyAny(T value) : storage(make_unique<Model<T>>(std::move(value))) {}

        // Copy constructor: deep-copies via the virtual clone().
        MyAny(const MyAny &other)
            : storage(other.storage ? other.storage->clone() : nullptr) {}

        // Copy assignment: same idea, via copy-and-swap-style reassignment.
        MyAny &operator=(const MyAny &other)
        {
            storage = other.storage ? other.storage->clone() : nullptr;
            return *this;
        }

        // Move construction/assignment are fine as compiler-generated since
        // storage is a unique_ptr.
        MyAny(MyAny &&) = default;
        MyAny &operator=(MyAny &&) = default;

        /*****************************************************************************
         * Name: hasValue
         *
         * Description:
         *         Reports whether this MyAny currently holds a value.
         *
         * Returns:
         *         True if a value is stored, false if empty.
         *****************************************************************************/
        bool hasValue() const { return storage != nullptr; }

        /*****************************************************************************
         * Name: heldType
         *
         * Description:
         *         Returns the type_info of the currently stored value.
         *
         * Returns:
         *         Reference to the stored value's std::type_info.
         *****************************************************************************/
        const type_info &heldType() const
        {
            if (!storage)
            {
                throw runtime_error("MyAny is empty");
            }
            return storage->type();
        }

        template <typename T>
        friend T anyCast(const MyAny &any);
};

/*****************************************************************************
 * Name: anyCast
 *
 * Description:
 *         Extracts a value of type T from a MyAny, comparing the requested
 *         type against the type erased inside it via typeid.
 *
 * Inputs:
 *         any : the MyAny to extract a value from.
 *
 * Returns:
 *         A copy of the stored value of type T.
 *****************************************************************************/
template <typename T>
T anyCast(const MyAny &any)
{
    if (!any.storage || (any.storage->type() != typeid(T)))
    {
        throw bad_cast();
    }
    // Safe: we just proved the dynamic type matches T via typeid comparison.
    auto *model = static_cast<MyAny::Model<T> *>(any.storage.get());
    return model->value;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Stores an int and a std::string in MyAny instances, then extracts
 *         them with anyCast, including a demonstration of the mismatch throw.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    MyAny intAny(42);
    MyAny stringAny(string("hello type erasure"));

    cout << "intAny = " << anyCast<int>(intAny) << "\n";
    cout << "stringAny = " << anyCast<string>(stringAny) << "\n";

    try
    {
        cout << anyCast<double>(intAny) << "\n";
    }
    catch (const bad_cast &e)
    {
        cout << "anyCast<double>(intAny) threw std::bad_cast as expected\n";
    }

    return 0;
}
