#include <iostream>
#include <memory>

using namespace std;

// Prototype: clone existing shapes instead of rebuilding them from scratch.
class Shape
{
public:
    virtual ~Shape() = default;
    virtual unique_ptr<Shape> clone() const = 0;
    virtual void draw() const = 0;
};

class Circle : public Shape
{
public:
    // trivial constructor
    explicit Circle(int radius) : radius(radius) {}

    // trivial override
    unique_ptr<Shape> clone() const override
    {
        return make_unique<Circle>(*this);
    }

    // trivial override
    void draw() const override
    {
        cout << "Circle with radius " << radius << endl;
    }

private:
    int radius;
};

class Rectangle : public Shape
{
public:
    // trivial constructor
    Rectangle(int width, int height) : width(width), height(height) {}

    // trivial override
    unique_ptr<Shape> clone() const override
    {
        return make_unique<Rectangle>(*this);
    }

    // trivial override
    void draw() const override
    {
        cout << "Rectangle " << width << "x" << height << endl;
    }

private:
    int width;
    int height;
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Creates prototype shapes, clones them, and draws both the
 *         originals and the clones to show they are independent copies.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    unique_ptr<Shape> circlePrototype = make_unique<Circle>(5);
    unique_ptr<Shape> rectPrototype = make_unique<Rectangle>(3, 4);

    unique_ptr<Shape> circleCopy = circlePrototype->clone();
    unique_ptr<Shape> rectCopy = rectPrototype->clone();

    circlePrototype->draw();
    circleCopy->draw();
    rectPrototype->draw();
    rectCopy->draw();
    return 0;
}
