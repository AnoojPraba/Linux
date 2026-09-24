#include <stdio.h>

struct Point
{
    int x;
    int y;
};

struct Rectangle
{
    struct Point topLeft;
    struct Point bottomRight;
};

int rectangleArea(struct Rectangle *r)
{
    int width = r->bottomRight.x - r->topLeft.x;
    int height = r->bottomRight.y - r->topLeft.y;

    return width * height;
}

int main()
{
    struct Rectangle rect;

    rect.topLeft.x = 0;
    rect.topLeft.y = 0;
    rect.bottomRight.x = 10;
    rect.bottomRight.y = 5;

    printf("Rectangle area = %d\n", rectangleArea(&rect));

    // Array of structs
    struct Point path[3] = {{0, 0}, {1, 2}, {3, 4}};
    int i;

    for (i = 0; i < 3; i++)
    {
        printf("path[%d] = (%d, %d)\n", i, path[i].x, path[i].y);
    }

    return 0;
}
