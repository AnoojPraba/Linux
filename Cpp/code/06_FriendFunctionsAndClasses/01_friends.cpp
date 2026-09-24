#include <iostream>

using namespace std;

// Demonstrates a friend function and a friend class, both granted access to
// a class's private members.
class Box
{
    private:
        double width;

        // Trivial constructor with initializer list.
        Box(double w) : width(w) {}

    public:
        // Public factory so main() can create a Box despite the private constructor.
        static Box create(double w) { return Box(w); }

        friend double getWidth(const Box &box);
        friend class BoxInspector;
};

/*****************************************************************************
 * Name: getWidth
 *
 * Description:
 *         Friend function granted direct access to Box's private width field.
 *
 * Inputs:
 *         box : the Box to inspect.
 *
 * Returns:
 *         The box's width.
 *****************************************************************************/
double getWidth(const Box &box)
{
    return box.width;
}

// Friend class granted direct access to Box's private members.
class BoxInspector
{
    public:
        /*****************************************************************************
         * Name: report
         *
         * Description:
         *         Prints the private width of a Box, made accessible via the
         *         friend class relationship.
         *
         * Inputs:
         *         box : the Box to report on.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void report(const Box &box) const
        {
            cout << "BoxInspector sees private width: " << box.width << "\n";
        }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Exercises the friend function and friend class access to a Box's
 *         private width member.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    Box box = Box::create(5.5);
    cout << "getWidth (friend function): " << getWidth(box) << "\n";

    BoxInspector inspector;
    inspector.report(box);
    return 0;
}
