#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int id;
    const char *name;
} Employee;

int compareById(const void *a, const void *b)
{
    const Employee *empA = (const Employee *)a;
    const Employee *empB = (const Employee *)b;

    return empA->id - empB->id;
}

int main()
{
    Employee employees[] = {
        {103, "Carol"}, {101, "Alice"}, {105, "Eve"}, {102, "Bob"}
    };
    int size = sizeof(employees) / sizeof(employees[0]);
    Employee target = {105, NULL};
    Employee *found;

    // bsearch requires the array already sorted by the same comparator it's
    // given - it has no way to check that itself, so sorting first with
    // qsort using the same comparator is the standard pairing.
    qsort(employees, size, sizeof(Employee), compareById);

    found = bsearch(&target, employees, size, sizeof(Employee), compareById);

    if (found != NULL)
    {
        printf("found employee %d: %s\n", found->id, found->name);
    }
    else
    {
        printf("employee %d not found\n", target.id);
    }

    return 0;
}
