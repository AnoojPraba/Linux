#include <iostream>
#include <memory>

// Prototype: clone existing shapes instead of rebuilding them from scratch.
class Shape
{
public:
    virtual ~Shape() = default;
    virtual std::unique_ptr<Shape> clone() const = 0;
    virtual void draw() const = 0;
};

class Circle : public Shape
{
public:
    // trivial constructor
    explicit Circle(int radius) : radius(radius) {}

    // trivial override
    std::unique_ptr<Shape> clone() const override
    {
        return std::make_unique<Circle>(*this);
    }

    // trivial override
    void draw() const override
    {
        std::cout << "Circle with radius " << radius << std::endl;
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
    std::unique_ptr<Shape> clone() const override
    {
        return std::make_unique<Rectangle>(*this);
    }

    // trivial override
    void draw() const override
    {
        std::cout << "Rectangle " << width << "x" << height << std::endl;
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
    std::unique_ptr<Shape> circlePrototype = std::make_unique<Circle>(5);
    std::unique_ptr<Shape> rectPrototype = std::make_unique<Rectangle>(3, 4);

    std::unique_ptr<Shape> circleCopy = circlePrototype->clone();
    std::unique_ptr<Shape> rectCopy = rectPrototype->clone();

    circlePrototype->draw();
    circleCopy->draw();
    rectPrototype->draw();
    rectCopy->draw();
    return 0;
}
