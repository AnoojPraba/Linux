#include <iostream>

using namespace std;

#define PI 3.14159265358979323846

// Demonstrates virtual functions, override, an abstract base class, and a
// pure virtual function.
class Shape
{
    public:
        // Pure virtual function makes Shape an abstract base class.
        virtual double area() const = 0;

        /*****************************************************************************
         * Name: describe
         *
         * Description:
         *         Prints the shape's computed area via the virtual area() function.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        virtual void describe() const
        {
            cout << "Shape with area " << area() << "\n";
        }

        virtual ~Shape() = default;
};

class Circle : public Shape
{
    private:
        double radius;

    public:
        // Trivial constructor with initializer list.
        Circle(double r) : radius(r) {}

        /*****************************************************************************
         * Name: area
         *
         * Description:
         *         Computes the area of the circle.
         *
         * Returns:
         *         The circle's area as a double.
         *****************************************************************************/
        double area() const override
        {
            return PI * radius * radius;
        }
};

class Rectangle : public Shape
{
    private:
        double width;
        double height;

    public:
        // Trivial constructor with initializer list.
        Rectangle(double w, double h) : width(w), height(h) {}

        /*****************************************************************************
         * Name: area
         *
         * Description:
         *         Computes the area of the rectangle.
         *
         * Returns:
         *         The rectangle's area as a double.
         *****************************************************************************/
        double area() const override
        {
            return width * height;
        }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Stores Circle and Rectangle instances via base-class pointers and
 *         invokes the virtual describe() function polymorphically.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    Circle circle(2.0);
    Rectangle rectangle(3.0, 4.0);
    Shape *shapes[] = {&circle, &rectangle};

    for (Shape *shape : shapes)
    {
        shape->describe();
    }
    return 0;
}
