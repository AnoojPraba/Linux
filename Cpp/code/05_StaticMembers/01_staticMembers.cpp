#include <iostream>

using namespace std;

// Demonstrates static data members and static member functions.
class Counter
{
    private:
        static int instanceCount;
        int id;

    public:
        /*****************************************************************************
         * Name: Counter
         *
         * Description:
         *         Constructs a Counter, incrementing the shared static instance
         *         count and recording this instance's unique id.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        Counter()
        {
            instanceCount += 1;
            id = instanceCount;
        }

        // Trivial getter.
        int getId() const { return id; }

        /*****************************************************************************
         * Name: getInstanceCount
         *
         * Description:
         *         Returns the number of Counter instances created so far. Static
         *         member function: callable without an instance.
         *
         * Returns:
         *         The current static instance count.
         *****************************************************************************/
        static int getInstanceCount()
        {
            return instanceCount;
        }
};

// Definition (and storage) of the static data member.
int Counter::instanceCount = 0;

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Creates several Counter instances and shows the shared static
 *         instance count changing across all of them.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    cout << "initial count: " << Counter::getInstanceCount() << "\n";

    Counter first;
    Counter second;
    Counter third;

    cout << "first id: " << first.getId() << ", second id: " << second.getId()
              << ", third id: " << third.getId() << "\n";
    cout << "final count: " << Counter::getInstanceCount() << "\n";
    return 0;
}
