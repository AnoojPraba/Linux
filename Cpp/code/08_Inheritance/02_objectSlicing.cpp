#include <iostream>
#include <string>

using namespace std;

// Demonstrates object slicing: a derived class's extra data is silently
// dropped when a derived object is copied into a base-type object BY VALUE.
class Base
{
    protected:
        string name;

    public:
        // Trivial constructor with initializer list.
        Base(const string &baseName) : name(baseName) {}

        /*****************************************************************************
         * Name: describe
         *
         * Description:
         *         Prints the base name. Not virtual on purpose here, so that
         *         both the sliced and unsliced call paths are visible below.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void describe() const
        {
            cout << "Base name: " << name << "\n";
        }
};

class Derived : public Base
{
    private:
        string extraInfo;

    public:
        /*****************************************************************************
         * Name: Derived
         *
         * Description:
         *         Constructs a Derived, chaining to the Base constructor and
         *         initializing the derived-only extraInfo field.
         *
         * Inputs:
         *         derivedName : forwarded to the Base constructor.
         *         info        : derived-only data, not present in Base.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        Derived(const string &derivedName, const string &info)
            : Base(derivedName), extraInfo(info)
        {
        }

        /*****************************************************************************
         * Name: describeFull
         *
         * Description:
         *         Prints both the base name and the derived-only extraInfo.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void describeFull() const
        {
            cout << "Derived name: " << name << ", extraInfo: " << extraInfo << "\n";
        }
};

/*****************************************************************************
 * Name: printByValue
 *
 * Description:
 *         Accepts a Base BY VALUE. Passing a Derived here slices it: only the
 *         Base subobject is copied, via Base's own copy constructor, which
 *         has no knowledge of Derived's extraInfo.
 *
 * Inputs:
 *         base : sliced copy of whatever was passed in.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void printByValue(Base base)
{
    base.describe();
}

/*****************************************************************************
 * Name: printByReference
 *
 * Description:
 *         Accepts a Base BY REFERENCE. No copy is made, so no slicing occurs
 *         -- but since describe() is not virtual, this still only prints the
 *         Base portion (a separate issue from slicing; see 10_Polymorphism
 *         for making calls like this dispatch to the derived override).
 *
 * Inputs:
 *         base : reference to the original object, not a copy.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void printByReference(const Base &base)
{
    base.describe();
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Shows a Derived object's extra data quietly disappearing when
 *         passed/assigned by value into a Base, and confirms the original
 *         Derived object (and describeFull() through a reference) is
 *         unaffected.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    Derived derivedObject("Rex", "Labrador");

    // Slicing: derivedObject is copied into a plain Base by value, dropping
    // extraInfo entirely. This compiles cleanly -- no error, just silently
    // wrong/incomplete behavior.
    Base slicedCopy = derivedObject;
    slicedCopy.describe();

    printByValue(derivedObject);

    // No slicing here: passing by reference/pointer keeps the full Derived
    // object alive and intact.
    printByReference(derivedObject);
    derivedObject.describeFull();
    return 0;
}
