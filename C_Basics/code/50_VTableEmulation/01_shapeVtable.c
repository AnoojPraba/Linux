#include <stdio.h>

// A hand-rolled vtable: a struct of function pointers, one per "virtual
// method" - this is close to literally what a C++ compiler generates
// under the hood for a class with virtual functions, just written out
// explicitly instead of hidden by the language.
typedef struct
{
    double (*area)(const void *self);
    const char *(*name)(const void *self);
} ShapeVtable;

// Every concrete "subclass" embeds a pointer to its own vtable as its
// first member - this is what makes dispatch through a common Shape *
// pointer possible: the caller doesn't need to know which concrete type
// it has, only that "whatever this is, it has a vtable pointer first".
typedef struct
{
    const ShapeVtable *vtable;
} Shape;

typedef struct
{
    Shape base;
    double radius;
} Circle;

typedef struct
{
    Shape base;
    double width;
    double height;
} Rectangle;

double circleArea(const void *self)
{
    const Circle *circle = (const Circle *)self;

    return 3.14159 * circle->radius * circle->radius;
}

const char *circleName(const void *self)
{
    (void)self;
    return "Circle";
}

double rectangleArea(const void *self)
{
    const Rectangle *rect = (const Rectangle *)self;

    return rect->width * rect->height;
}

const char *rectangleName(const void *self)
{
    (void)self;
    return "Rectangle";
}

static const ShapeVtable circleVtable = {circleArea, circleName};
static const ShapeVtable rectangleVtable = {rectangleArea, rectangleName};

// The dispatch itself: shape->vtable->area(shape) looks up the function
// pointer through the vtable and calls it - the same indirection a C++
// virtual call compiles down to, just spelled out rather than implicit.
void printShapeInfo(const Shape *shape)
{
    printf("%s: area = %f\n", shape->vtable->name(shape), shape->vtable->area(shape));
}

int main()
{
    Circle circle;
    Rectangle rectangle;
    const Shape *shapes[2];

    circle.base.vtable = &circleVtable;
    circle.radius = 3.0;

    rectangle.base.vtable = &rectangleVtable;
    rectangle.width = 4.0;
    rectangle.height = 5.0;

    shapes[0] = (const Shape *)&circle;
    shapes[1] = (const Shape *)&rectangle;

    printShapeInfo(shapes[0]);
    printShapeInfo(shapes[1]);

    return 0;
}
