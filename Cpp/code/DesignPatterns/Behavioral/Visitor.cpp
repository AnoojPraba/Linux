#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class Circle;
class Square;

// Visitor: declares a visit overload per concrete element type (double dispatch).
class ShapeVisitor
{
public:
    virtual ~ShapeVisitor() = default;
    virtual void visit(const Circle& circle) const = 0;
    virtual void visit(const Square& square) const = 0;
};

// Element: accepts a visitor and forwards itself so the correct overload runs.
class Shape
{
public:
    virtual ~Shape() = default;
    virtual void accept(const ShapeVisitor& visitor) const = 0;
};

class Circle : public Shape
{
public:
    // ctor stores the radius
    explicit Circle(double radiusIn) : radius(radiusIn) {}

    // trivial getter
    double getRadius() const
    {
        return radius;
    }

    // trivial override
    void accept(const ShapeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }

private:
    double radius;
};

class Square : public Shape
{
public:
    // ctor stores the side length
    explicit Square(double sideIn) : side(sideIn) {}

    // trivial getter
    double getSide() const
    {
        return side;
    }

    // trivial override
    void accept(const ShapeVisitor& visitor) const override
    {
        visitor.visit(*this);
    }

private:
    double side;
};

#define PI 3.14159265358979323846

// Concrete visitor: computes and prints area, without modifying Shape classes.
class AreaVisitor : public ShapeVisitor
{
public:
    /*****************************************************************************
     * Name: visit
     *
     * Description:
     *         Computes and prints the area of a circle.
     *
     * Inputs:
     *         circle : the circle to measure.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void visit(const Circle& circle) const override
    {
        double area = PI * circle.getRadius() * circle.getRadius();
        cout << "Circle area: " << area << endl;
    }

    /*****************************************************************************
     * Name: visit
     *
     * Description:
     *         Computes and prints the area of a square.
     *
     * Inputs:
     *         square : the square to measure.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void visit(const Square& square) const override
    {
        double area = square.getSide() * square.getSide();
        cout << "Square area: " << area << endl;
    }
};

// Concrete visitor: exports each shape as an XML fragment, a second operation
// added without touching the Shape hierarchy at all.
class XmlExportVisitor : public ShapeVisitor
{
public:
    // trivial override
    void visit(const Circle& circle) const override
    {
        cout << "<circle radius=\"" << circle.getRadius() << "\"/>" << endl;
    }

    // trivial override
    void visit(const Square& square) const override
    {
        cout << "<square side=\"" << square.getSide() << "\"/>" << endl;
    }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Builds a heterogeneous shape collection and runs two independent
 *         visitors (area calculation and XML export) over it, showing new
 *         operations can be added without changing the Shape classes.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    vector<unique_ptr<Shape>> shapes;
    shapes.push_back(make_unique<Circle>(2.0));
    shapes.push_back(make_unique<Square>(3.0));

    AreaVisitor areaVisitor;
    for (const auto& shape : shapes)
    {
        shape->accept(areaVisitor);
    }

    XmlExportVisitor xmlVisitor;
    for (const auto& shape : shapes)
    {
        shape->accept(xmlVisitor);
    }
    return 0;
}
