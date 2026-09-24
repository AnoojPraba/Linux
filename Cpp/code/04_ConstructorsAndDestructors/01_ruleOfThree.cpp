#include <iostream>
#include <cstring>

using namespace std;

// Demonstrates default, parameterized, and copy constructors, the destructor,
// initializer lists, and the "rule of three" for a class that owns a raw
// resource (a heap-allocated buffer).
class Buffer
{
    private:
        char *data;
        size_t length;

    public:
        // Default constructor.
        Buffer() : data(nullptr), length(0) {}

        /*****************************************************************************
         * Name: Buffer (parameterized constructor)
         *
         * Description:
         *         Allocates a heap buffer and copies the given C string into it.
         *
         * Inputs:
         *         text : null-terminated string to copy into the new buffer.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        explicit Buffer(const char *text) : length(strlen(text))
        {
            data = new char[length + 1];
            strcpy(data, text);
        }

        /*****************************************************************************
         * Name: Buffer (copy constructor)
         *
         * Description:
         *         Performs a deep copy of another Buffer's owned memory.
         *
         * Inputs:
         *         other : the Buffer instance to copy from.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        Buffer(const Buffer &other) : length(other.length)
        {
            data = new char[length + 1];
            strcpy(data, other.data);
        }

        /*****************************************************************************
         * Name: operator=
         *
         * Description:
         *         Copy-assigns another Buffer, releasing any previously owned memory
         *         and performing a deep copy of the source.
         *
         * Inputs:
         *         other : the Buffer instance to assign from.
         *
         * Returns:
         *         Reference to this Buffer.
         *****************************************************************************/
        Buffer &operator=(const Buffer &other)
        {
            if (this == &other)
            {
                return *this;
            }
            delete[] data;
            length = other.length;
            data = new char[length + 1];
            strcpy(data, other.data);
            return *this;
        }

        /*****************************************************************************
         * Name: ~Buffer
         *
         * Description:
         *         Releases the heap memory owned by this Buffer.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        ~Buffer()
        {
            delete[] data;
        }

        // Trivial getter.
        const char *c_str() const { return data ? data : ""; }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Exercises the parameterized constructor, copy constructor, and copy
 *         assignment operator of Buffer, printing results at each step.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    Buffer original("hello");
    Buffer copyConstructed(original);
    Buffer copyAssigned;
    copyAssigned = original;

    cout << "original: " << original.c_str() << "\n";
    cout << "copyConstructed: " << copyConstructed.c_str() << "\n";
    cout << "copyAssigned: " << copyAssigned.c_str() << "\n";
    return 0;
}
