#include <iostream>
#include <string>

using namespace std;

// Demonstrates a function template and a class template.

/*****************************************************************************
 * Name: maxOf
 *
 * Description:
 *         Returns the larger of two values of any comparable type.
 *
 * Inputs:
 *         a : the first value.
 *         b : the second value.
 *
 * Returns:
 *         The larger of a and b.
 *****************************************************************************/
template <typename T>
T maxOf(T a, T b)
{
    return (a > b) ? a : b;
}

template <typename T>
class Box
{
    private:
        T contents;

    public:
        // Trivial constructor with initializer list.
        Box(T value) : contents(value) {}

        // Trivial getter.
        T get() const { return contents; }

        /*****************************************************************************
         * Name: set
         *
         * Description:
         *         Replaces the contents of the box with a new value.
         *
         * Inputs:
         *         value : the new value to store.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void set(T value)
        {
            contents = value;
        }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Exercises the maxOf function template with different types, and the
 *         Box class template.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    cout << "maxOf(3, 7) = " << maxOf(3, 7) << "\n";
    cout << "maxOf(2.5, 1.5) = " << maxOf(2.5, 1.5) << "\n";

    Box<int> intBox(42);
    Box<string> stringBox("hello");
    cout << "intBox holds " << intBox.get() << "\n";
    cout << "stringBox holds " << stringBox.get() << "\n";

    intBox.set(100);
    cout << "intBox now holds " << intBox.get() << "\n";
    return 0;
}
