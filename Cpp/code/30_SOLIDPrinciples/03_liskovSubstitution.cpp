#include <cassert>
#include <iostream>

#define RECT_WIDTH 4
#define RECT_HEIGHT 5
#define SQUARE_SIDE 4

// VIOLATION: Square "is-a" Rectangle in the math sense, but forcing that
// inheritance breaks the base class's contract: setting width on a Square
// silently changes its height too, so code written against Rectangle
// (assuming width/height are independent) misbehaves when given a Square.
class RectangleBad
{
public:
    // trivial ctor
    RectangleBad(int w, int h) : width(w), height(h) {}

    // trivial setter
    virtual void setWidth(int w)
    {
        width = w;
    }

    // trivial setter
    virtual void setHeight(int h)
    {
        height = h;
    }

    // trivial getter
    int area() const
    {
        return width * height;
    }

protected:
    int width;
    int height;
};

class SquareBad : public RectangleBad
{
public:
    // trivial ctor
    explicit SquareBad(int s) : RectangleBad(s, s) {}

    // breaks the base contract: width and height are no longer independent
    void setWidth(int w) override
    {
        width = w;
        height = w;
    }

    // breaks the base contract: width and height are no longer independent
    void setHeight(int h) override
    {
        width = h;
        height = h;
    }
};

/*****************************************************************************
 * Name: resizeAndCheck
 *
 * Description:
 *         Assumes width and height can be set independently, which is a
 *         valid assumption for RectangleBad but is violated by SquareBad.
 *
 * Inputs:
 *         rect : the rectangle-like object to resize.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void resizeAndCheck(RectangleBad &rect)
{
    rect.setWidth(RECT_WIDTH);
    rect.setHeight(RECT_HEIGHT);
    // Fails for SquareBad since height overwrote width above.
    assert(rect.area() == (RECT_WIDTH * RECT_HEIGHT));
}

// FIX: don't force an is-a relationship where behavior isn't substitutable.
// Model the common capability (having an area) via a shared interface
// instead, so callers never assume independent width/height.
class ShapeWithArea
{
public:
    virtual ~ShapeWithArea() = default;
    virtual int area() const = 0;
};

class Rectangle : public ShapeWithArea
{
public:
    // trivial ctor
    Rectangle(int w, int h) : width(w), height(h) {}

    // trivial override
    int area() const override
    {
        return width * height;
    }

private:
    int width;
    int height;
};

class Square : public ShapeWithArea
{
public:
    // trivial ctor
    explicit Square(int s) : side(s) {}

    // trivial override
    int area() const override
    {
        return side * side;
    }

private:
    int side;
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates the LSP violation (skipped to avoid an assertion
 *         failure) followed by the fixed design where Rectangle and Square
 *         are both substitutable through ShapeWithArea without any hidden
 *         contract breakage.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    RectangleBad rect(1, 1);
    resizeAndCheck(rect);
    std::cout << "RectangleBad resized correctly, area = " << rect.area() << std::endl;
    // resizeAndCheck(SquareBad(1)) would trip the assert above -- not called here.

    Rectangle goodRect(RECT_WIDTH, RECT_HEIGHT);
    Square goodSquare(SQUARE_SIDE);
    std::cout << "Rectangle area: " << goodRect.area() << std::endl;
    std::cout << "Square area: " << goodSquare.area() << std::endl;

    return 0;
}
