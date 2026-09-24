#include <iostream>

// CRTP (Curiously Recurring Template Pattern): a base class template is parameterized
// on its own derived class, allowing static (compile-time) polymorphism without the
// runtime cost of virtual dispatch.

using namespace std;

template <typename Derived>
class Shape
{
    public:
        /*****************************************************************************
         * Name: draw
         *
         * Description:
         *         Dispatches to the derived class's drawImpl() at compile
         *         time via a static_cast, avoiding a virtual function call.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void draw() const
        {
            static_cast<const Derived *>(this)->drawImpl();
        }
};

class Circle : public Shape<Circle>
{
    public:
        // trivial
        void drawImpl() const
        {
            cout << "drawing a circle\n";
        }
};

class Square : public Shape<Square>
{
    public:
        // trivial
        void drawImpl() const
        {
            cout << "drawing a square\n";
        }
};

/*****************************************************************************
 * Name: renderShape
 *
 * Description:
 *         Renders any Shape<Derived> by calling its draw() method, with the
 *         concrete type resolved entirely at compile time.
 *
 * Inputs:
 *         shape : the CRTP shape to render.
 *
 * Returns:
 *         None.
 *****************************************************************************/
template <typename Derived>
void renderShape(const Shape<Derived> &shape)
{
    shape.draw();
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Renders a Circle and a Square through the CRTP base class,
 *         showing static polymorphism resolved without virtual functions.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    Circle circle;
    Square square;

    renderShape(circle);
    renderShape(square);

    return 0;
}
