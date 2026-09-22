#include <iostream>
#include <memory>
#include <vector>

#define CIRCLE_RADIUS 3.0
#define RECTANGLE_WIDTH 4.0
#define RECTANGLE_HEIGHT 5.0
#define PI 3.14159

// VIOLATION: adding a new shape means editing this function's if/else chain.
enum class ShapeKind
{
    Circle,
    Rectangle
};

struct ShapeData
{
    ShapeKind kind;
    double a;
    double b;
};

/*****************************************************************************
 * Name: areaBad
 *
 * Description:
 *         Computes area by switching on shape kind. Every new shape type
 *         requires modifying this existing function, violating OCP.
 *
 * Inputs:
 *         shape : the shape data to compute the area for.
 *
 * Returns:
 *         The computed area, or 0.0 for an unknown kind.
 *****************************************************************************/
double areaBad(const ShapeData &shape)
{
    if (shape.kind == ShapeKind::Circle)
    {
        return PI * shape.a * shape.a;
    }
    else if (shape.kind == ShapeKind::Rectangle)
    {
        return shape.a * shape.b;
    }
    return 0.0;
}

// FIX: new shapes are added by creating a new subclass, without touching
// existing code (open for extension, closed for modification).
class Shape
{
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

class Circle : public Shape
{
public:
    // trivial ctor
    explicit Circle(double r) : radius(r) {}

    // trivial override
    double area() const override
    {
        return PI * radius * radius;
    }

private:
    double radius;
};

class Rectangle : public Shape
{
public:
    // trivial ctor
    Rectangle(double w, double h) : width(w), height(h) {}

    // trivial override
    double area() const override
    {
        return width * height;
    }

private:
    double width;
    double height;
};

/*****************************************************************************
 * Name: totalArea
 *
 * Description:
 *         Sums the areas of a collection of shapes polymorphically. Never
 *         needs modification when a new Shape subclass is introduced.
 *
 * Inputs:
 *         shapes : the collection of shapes to sum.
 *
 * Returns:
 *         The total area across all shapes.
 *****************************************************************************/
double totalArea(const std::vector<std::unique_ptr<Shape>> &shapes)
{
    double sum = 0.0;
    for (const auto &shape : shapes)
    {
        sum += shape->area();
    }
    return sum;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates the OCP violation followed by the fixed polymorphic
 *         design.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    ShapeData circleData{ShapeKind::Circle, CIRCLE_RADIUS, 0.0};
    ShapeData rectData{ShapeKind::Rectangle, RECTANGLE_WIDTH, RECTANGLE_HEIGHT};
    std::cout << "Bad circle area: " << areaBad(circleData) << std::endl;
    std::cout << "Bad rectangle area: " << areaBad(rectData) << std::endl;

    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(CIRCLE_RADIUS));
    shapes.push_back(std::make_unique<Rectangle>(RECTANGLE_WIDTH, RECTANGLE_HEIGHT));
    std::cout << "Total area: " << totalArea(shapes) << std::endl;

    return 0;
}
