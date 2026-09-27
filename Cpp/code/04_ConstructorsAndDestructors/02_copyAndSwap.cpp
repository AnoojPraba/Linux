#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;

// Demonstrates the copy-and-swap idiom for exception-safe operator=.
class SwapBuffer
{
    private:
        char *data;
        size_t length;

    public:
        // Trivial default constructor.
        SwapBuffer() : data(nullptr), length(0) {}

        /*****************************************************************************
         * Name: SwapBuffer (parameterized constructor)
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
        explicit SwapBuffer(const char *text) : length(strlen(text))
        {
            data = new char[length + 1];
            strcpy(data, text);
        }

        // Copy constructor: deep copy, may throw std::bad_alloc.
        SwapBuffer(const SwapBuffer &other) : length(other.length)
        {
            data = new char[length + 1];
            strcpy(data, other.data);
        }

        /*****************************************************************************
         * Name: swap (member function)
         *
         * Description:
         *         Exchanges the contents of this SwapBuffer with another, using
         *         only non-throwing operations (pointer/scalar swaps).
         *
         * Inputs:
         *         other : the SwapBuffer to swap contents with.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void swap(SwapBuffer &other) noexcept
        {
            std::swap(data, other.data);
            std::swap(length, other.length);
        }

        // Copy-and-swap operator=: takes its argument BY VALUE, so the copy
        // (which may throw) happens while constructing "other", entirely before
        // this function's body runs. If that copy throws, *this has not been
        // touched at all -- strong exception safety for free. The swap() call
        // itself is noexcept, so once we reach it nothing can go wrong. This
        // also handles self-assignment correctly without an explicit
        // "if (this == &other)" check: assigning from itself just copies into
        // the by-value parameter and swaps back, a harmless no-op.
        SwapBuffer &operator=(SwapBuffer other)
        {
            this->swap(other);
            return *this;
        }

        /*****************************************************************************
         * Name: swap (free function)
         *
         * Description:
         *         Non-member swap enabling argument-dependent lookup (ADL) and
         *         matching the std::swap(a, b) idiom.
         *
         * Inputs:
         *         a : first SwapBuffer.
         *         b : second SwapBuffer.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        friend void swap(SwapBuffer &a, SwapBuffer &b) noexcept
        {
            a.swap(b);
        }

        // Trivial destructor.
        ~SwapBuffer()
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
 *         Exercises copy-and-swap assignment, including self-assignment.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    SwapBuffer first("hello");
    SwapBuffer second("world");

    second = first;
    cout << "second after assignment: " << second.c_str() << "\n";

    first = first;
    cout << "first after self-assignment: " << first.c_str() << "\n";
    return 0;
}
