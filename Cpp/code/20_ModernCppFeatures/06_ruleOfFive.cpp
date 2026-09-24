#include <iostream>
#include <cstring>
#include <utility>

using namespace std;

// Rule of five: any class that manages a raw resource and needs a custom
// destructor should also define (or explicitly delete) the copy
// constructor, copy assignment, move constructor, and move assignment.

class Buffer
{
    private:
        char *data;

    public:
        /*****************************************************************************
         * Name: Buffer
         *
         * Description:
         *         Allocates and copies the given text into an owned buffer.
         *
         * Inputs:
         *         text : the null-terminated string to copy.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        explicit Buffer(const char *text)
        {
            data = new char[strlen(text) + 1];
            strcpy(data, text);
            cout << "constructor ran\n";
        }

        /*****************************************************************************
         * Name: Buffer (copy constructor)
         *
         * Description:
         *         Deep-copies another Buffer's owned data.
         *
         * Inputs:
         *         other : the Buffer to copy from.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        Buffer(const Buffer &other)
        {
            data = new char[strlen(other.data) + 1];
            strcpy(data, other.data);
            cout << "copy constructor ran\n";
        }

        /*****************************************************************************
         * Name: operator= (copy assignment)
         *
         * Description:
         *         Releases this Buffer's data and deep-copies another
         *         Buffer's owned data in its place.
         *
         * Inputs:
         *         other : the Buffer to copy from.
         *
         * Returns:
         *         A reference to this Buffer.
         *****************************************************************************/
        Buffer &operator=(const Buffer &other)
        {
            cout << "copy assignment ran\n";
            if (this != &other)
            {
                delete[] data;
                data = new char[strlen(other.data) + 1];
                strcpy(data, other.data);
            }
            return *this;
        }

        /*****************************************************************************
         * Name: Buffer (move constructor)
         *
         * Description:
         *         Steals another Buffer's owned pointer, leaving the source
         *         in a valid but empty state.
         *
         * Inputs:
         *         other : the Buffer to move from.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        Buffer(Buffer &&other) noexcept
        {
            data = other.data;
            other.data = nullptr;
            cout << "move constructor ran\n";
        }

        /*****************************************************************************
         * Name: operator= (move assignment)
         *
         * Description:
         *         Releases this Buffer's data and steals another Buffer's
         *         owned pointer, leaving the source in a valid empty state.
         *
         * Inputs:
         *         other : the Buffer to move from.
         *
         * Returns:
         *         A reference to this Buffer.
         *****************************************************************************/
        Buffer &operator=(Buffer &&other) noexcept
        {
            cout << "move assignment ran\n";
            if (this != &other)
            {
                delete[] data;
                data = other.data;
                other.data = nullptr;
            }
            return *this;
        }

        /*****************************************************************************
         * Name: ~Buffer
         *
         * Description:
         *         Releases the owned buffer, if any.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        ~Buffer()
        {
            cout << "destructor ran\n";
            delete[] data;
        }
};

/*****************************************************************************
 * Name: makeBuffer
 *
 * Description:
 *         Returns a Buffer by value so the caller can observe move
 *         construction taking place on return.
 *
 * Returns:
 *         A newly constructed Buffer.
 *****************************************************************************/
Buffer makeBuffer()
{
    return Buffer("temporary");
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Exercises every rule-of-five special member function on Buffer,
 *         printing which one runs at each step.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    Buffer original("original data");
    Buffer copy(original);

    Buffer assigned("placeholder");
    assigned = original;

    Buffer moved(std::move(original));

    Buffer moveAssigned("placeholder2");
    moveAssigned = makeBuffer();

    return 0;
}
