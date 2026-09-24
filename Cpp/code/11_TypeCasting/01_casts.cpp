#include <iostream>

using namespace std;

// Demonstrates static_cast, dynamic_cast, const_cast, and reinterpret_cast.
class Base
{
    public:
        virtual ~Base() = default;

        /*****************************************************************************
         * Name: identify
         *
         * Description:
         *         Prints a message identifying this object as a Base.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        virtual void identify() const
        {
            cout << "I am a Base\n";
        }
};

class Derived : public Base
{
    public:
        /*****************************************************************************
         * Name: identify
         *
         * Description:
         *         Prints a message identifying this object as a Derived.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void identify() const override
        {
            cout << "I am a Derived\n";
        }

        /*****************************************************************************
         * Name: derivedOnly
         *
         * Description:
         *         Prints a message that only exists on Derived, used to prove a
         *         dynamic_cast succeeded.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void derivedOnly() const
        {
            cout << "derived-only behavior\n";
        }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Exercises static_cast for numeric conversion, dynamic_cast for safe
 *         downcasting, const_cast for removing constness, and
 *         reinterpret_cast for reinterpreting a pointer's type.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    double pi = 3.9;
    int truncated = static_cast<int>(pi);
    cout << "static_cast<int>(3.9) = " << truncated << "\n";

    Base *basePtr = new Derived();
    Derived *derivedPtr = dynamic_cast<Derived *>(basePtr);
    if (derivedPtr != nullptr)
    {
        derivedPtr->derivedOnly();
    }
    delete basePtr;

    const int constValue = 42;
    int &mutableRef = const_cast<int &>(constValue);
    mutableRef = 100;
    cout << "after const_cast modification, constValue = " << constValue << "\n";

    int number = 65;
    char *asBytes = reinterpret_cast<char *>(&number);
    cout << "reinterpret_cast first byte of int as char: " << asBytes[0] << "\n";
    return 0;
}
