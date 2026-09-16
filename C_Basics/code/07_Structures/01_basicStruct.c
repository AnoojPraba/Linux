#include <stdio.h>
#include <string.h>

struct Point
{
    int x;
    int y;
};

typedef struct
{
    char name[32];
    int age;
} Person;

void movePoint(struct Point *p, int dx, int dy)
{
    p->x += dx;
    p->y += dy;
}

int main()
{
    struct Point origin = {0, 0};
    Person alice;

    movePoint(&origin, 3, 4);
    printf("origin after move: (%d, %d)\n", origin.x, origin.y);

    strcpy(alice.name, "Alice");
    alice.age = 30;
    printf("Person: %s, age %d\n", alice.name, alice.age);

    // Struct assignment copies every member (a shallow, member-by-member copy)
    struct Point copy = origin;

    copy.x = 100;
    printf("\ncopy.x = %d, origin.x = %d (copy is independent)\n", copy.x, origin.x);

    return 0;
}
