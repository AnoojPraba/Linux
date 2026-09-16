#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char *name;
    int age;
} Person;

int main()
{
    int count = 3;
    Person *people = malloc(count * sizeof(Person));
    int i;
    const char *names[] = {"Alice", "Bob", "Carol"};

    for (i = 0; i < count; i++)
    {
        // Each struct owns its own malloc'd string, so freeing the array
        // later requires freeing every member allocation first, then the array.
        people[i].name = malloc(strlen(names[i]) + 1);
        strcpy(people[i].name, names[i]);
        people[i].age = 20 + i;
    }

    for (i = 0; i < count; i++)
    {
        printf("%s is %d\n", people[i].name, people[i].age);
    }

    for (i = 0; i < count; i++)
    {
        free(people[i].name);
    }
    free(people);

    return 0;
}
