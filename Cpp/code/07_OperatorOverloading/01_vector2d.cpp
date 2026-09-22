#include <iostream>

#define VEC_COMPONENTS 2

// Demonstrates overloading +, ==, <<, and [] for a simple 2D vector class.
class Vector2D
{
    private:
        double values[VEC_COMPONENTS];

    public:
        // Trivial constructor with initializer list.
        Vector2D(double x, double y) : values{x, y} {}

        /*****************************************************************************
         * Name: operator+
         *
         * Description:
         *         Adds two Vector2D instances component-wise.
         *
         * Inputs:
         *         other : the Vector2D to add to this one.
         *
         * Returns:
         *         A new Vector2D holding the component-wise sum.
         *****************************************************************************/
        Vector2D operator+(const Vector2D &other) const
        {
            return Vector2D(values[0] + other.values[0], values[1] + other.values[1]);
        }

        /*****************************************************************************
         * Name: operator==
         *
         * Description:
         *         Compares two Vector2D instances for exact component-wise equality.
         *
         * Inputs:
         *         other : the Vector2D to compare against.
         *
         * Returns:
         *         true if both components match, false otherwise.
         *****************************************************************************/
        bool operator==(const Vector2D &other) const
        {
            return ((values[0] == other.values[0]) && (values[1] == other.values[1]));
        }

        /*****************************************************************************
         * Name: operator[]
         *
         * Description:
         *         Provides indexed access to the vector's components.
         *
         * Inputs:
         *         index : 0 for x, 1 for y.
         *
         * Returns:
         *         Reference to the requested component.
         *****************************************************************************/
        double &operator[](int index)
        {
            return values[index];
        }

        // Trivial getters used by operator<<.
        double x() const { return values[0]; }
        double y() const { return values[1]; }
};

/*****************************************************************************
 * Name: operator<<
 *
 * Description:
 *         Streams a Vector2D to an output stream in "(x, y)" format.
 *
 * Inputs:
 *         out : the output stream to write to.
 *         vec : the Vector2D to print.
 *
 * Returns:
 *         Reference to the output stream, to allow chaining.
 *****************************************************************************/
std::ostream &operator<<(std::ostream &out, const Vector2D &vec)
{
    out << "(" << vec.x() << ", " << vec.y() << ")";
    return out;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Exercises the overloaded operators on Vector2D instances.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    Vector2D a(1.0, 2.0);
    Vector2D b(3.0, 4.0);
    Vector2D sum = a + b;

    std::cout << "a = " << a << ", b = " << b << ", a + b = " << sum << "\n";
    std::cout << "a == a? " << (a == a) << ", a == b? " << (a == b) << "\n";

    sum[0] = 99.0;
    std::cout << "sum after sum[0] = 99: " << sum << "\n";
    return 0;
}
