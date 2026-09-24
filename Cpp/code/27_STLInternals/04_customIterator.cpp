#include <iostream>
#include <iterator>

#define CONTAINER_CAPACITY 8

using namespace std;

// A minimal fixed-capacity container with a custom InputIterator-like iterator,
// satisfying just enough of the iterator requirements to work with range-based for
// and a handful of <algorithm> functions.
class IntBox
{
    private:
        int elements[CONTAINER_CAPACITY];
        int count;

    public:
        // trivial constructor
        IntBox() : count(0) {}

        /*****************************************************************************
         * Name: add
         *
         * Description:
         *         Appends a value to the box if capacity remains.
         *
         * Inputs:
         *         value : the value to append.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void add(int value)
        {
            if (count < CONTAINER_CAPACITY)
            {
                elements[count] = value;
                count = count + 1;
            }
        }

        // Custom iterator satisfying basic InputIterator-like requirements:
        // dereferenceable, incrementable, and comparable for equality/inequality.
        class Iterator
        {
            public:
                using iterator_category = input_iterator_tag;
                using value_type = int;
                using difference_type = ptrdiff_t;
                using pointer = const int *;
                using reference = const int &;

                // trivial constructor
                explicit Iterator(const int *ptr) : current(ptr) {}

                // trivial dereference
                reference operator*() const { return *current; }

                // trivial pre-increment
                Iterator &operator++()
                {
                    current = current + 1;
                    return *this;
                }

                // trivial equality comparison
                bool operator==(const Iterator &other) const { return current == other.current; }

                // trivial inequality comparison
                bool operator!=(const Iterator &other) const { return current != other.current; }

            private:
                const int *current;
        };

        // trivial begin
        Iterator begin() const { return Iterator(elements); }

        // trivial end
        Iterator end() const { return Iterator(elements + count); }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Fills an IntBox and iterates it with both a range-based for loop
 *         and an explicit iterator loop, relying only on the custom
 *         iterator's begin, end, dereference, increment, and inequality
 *         operations.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    IntBox box;
    box.add(10);
    box.add(20);
    box.add(30);

    for (int value : box)
    {
        cout << "value = " << value << "\n";
    }

    int total = 0;
    for (auto it = box.begin(); it != box.end(); ++it)
    {
        total = total + *it;
    }
    cout << "total = " << total << "\n";

    return 0;
}
