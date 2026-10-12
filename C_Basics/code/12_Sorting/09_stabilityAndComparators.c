// Stable versus unstable sorting, multi-key comparators, sorting strings and
// indirect sorting by index.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { const char *name; int grade; int order; } Student;

static int by_grade(const void *a, const void *b) { return ((const Student *)a)->grade - ((const Student *)b)->grade; }

static int by_grade_then_name(const void *a, const void *b)
{
    const Student *x = a, *y = b;
    if (x->grade != y->grade) return x->grade - y->grade;
    return strcmp(x->name, y->name);                       /* ties broken by a second key */
}

static int by_grade_stable(const void *a, const void *b)   /* qsort is not guaranteed stable: add the original index as the last key */
{
    const Student *x = a, *y = b;
    if (x->grade != y->grade) return x->grade - y->grade;
    return x->order - y->order;
}

static int str_cmp(const void *a, const void *b) { return strcmp(*(const char **)a, *(const char **)b); }

int main(void)
{
    Student s[] = {{"ann", 2, 0}, {"raj", 1, 1}, {"li", 2, 2}, {"sam", 1, 3}, {"zoe", 2, 4}};
    qsort(s, 5, sizeof s[0], by_grade_stable);
    printf("stable by grade       :");
    for (int i = 0; i < 5; i++) printf(" %s(%d)", s[i].name, s[i].grade);
    qsort(s, 5, sizeof s[0], by_grade_then_name);
    printf("\ngrade then name       :");
    for (int i = 0; i < 5; i++) printf(" %s(%d)", s[i].name, s[i].grade);
    qsort(s, 5, sizeof s[0], by_grade);
    printf("\nby grade only (order of equal grades is unspecified for qsort; yours may differ):");
    for (int i = 0; i < 5; i++) printf(" %d", s[i].grade);

    const char *words[] = {"pear", "apple", "fig", "banana"};
    qsort(words, 4, sizeof words[0], str_cmp);              /* sorting an array of pointers moves pointers, not strings */
    printf("\nstrings:");
    for (int i = 0; i < 4; i++) printf(" %s", words[i]);

    int values[] = {30, 10, 20}, idx[] = {0, 1, 2};
    for (int i = 0; i < 3; i++)                             /* indirect sort: order the indexes, leave the data in place */
        for (int j = i + 1; j < 3; j++)
            if (values[idx[j]] < values[idx[i]]) { int t = idx[i]; idx[i] = idx[j]; idx[j] = t; }
    printf("\nindirect sort order of {30,10,20}: %d %d %d\n", idx[0], idx[1], idx[2]);
    return 0;
}
