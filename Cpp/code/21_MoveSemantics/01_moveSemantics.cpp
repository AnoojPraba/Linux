#include <iostream>
#include <utility>

// Demonstrates move constructor, move assignment, std::move, and rvalue
// references for a class that owns a heap-allocated array.
class IntArray
{
    private:
        int *data;
        size_t size;

    public:
        /*****************************************************************************
         * Name: IntArray (parameterized constructor)
         *
         * Description:
         *         Allocates an array of the given size, zero-initialized.
         *
         * Inputs:
         *         count : number of ints to allocate.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        explicit IntArray(size_t count) : size(count)
        {
            data = new int[size]();
            std::cout << "constructed IntArray of size " << size << "\n";
        }

        /*****************************************************************************
         * Name: IntArray (move constructor)
         *
         * Description:
         *         Steals the internal buffer from an rvalue IntArray instead of
         *         performing a deep copy, leaving the source in a valid empty state.
         *
         * Inputs:
         *         other : the rvalue IntArray to move from.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        IntArray(IntArray &&other) noexcept : data(other.data), size(other.size)
        {
            other.data = nullptr;
            other.size = 0;
            std::cout << "move-constructed IntArray\n";
        }

        /*****************************************************************************
         * Name: operator= (move assignment)
         *
         * Description:
         *         Releases this instance's buffer and steals the source's buffer.
         *
         * Inputs:
         *         other : the rvalue IntArray to move from.
         *
         * Returns:
         *         Reference to this IntArray.
         *****************************************************************************/
        IntArray &operator=(IntArray &&other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
            std::cout << "move-assigned IntArray\n";
            return *this;
        }

        IntArray(const IntArray &) = delete;
        IntArray &operator=(const IntArray &) = delete;

        /*****************************************************************************
         * Name: ~IntArray
         *
         * Description:
         *         Releases the owned heap buffer, if any.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        ~IntArray()
        {
            delete[] data;
        }

        // Trivial getter.
        size_t getSize() const { return size; }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Creates an IntArray, moves it into another via std::move, and
 *         observes the resulting ownership transfer.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    IntArray first(10);
    IntArray second(std::move(first));
    std::cout << "second size: " << second.getSize() << ", first size: "
              << first.getSize() << "\n";

    IntArray third(5);
    third = std::move(second);
    std::cout << "third size: " << third.getSize() << ", second size: "
              << second.getSize() << "\n";
    return 0;
}
