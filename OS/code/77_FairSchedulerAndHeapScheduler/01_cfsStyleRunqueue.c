// Tasks with weights 1024/1024/512 picked by smallest vruntime; the lighter
// task gets half the CPU.

#include <stdio.h>
#include <stdlib.h>

typedef struct Task
{
    long vruntime;
    int weight;
    const char *name;
    int height;
    struct Task *left, *right;
} Task;

static int height(Task *t) { return t ? t->height : 0; }
static void update(Task *t) { t->height = 1 + (height(t->left) > height(t->right) ? height(t->left) : height(t->right)); }

static Task *rot_right(Task *y)
{
    Task *x = y->left;
    y->left = x->right;
    x->right = y;
    update(y);
    update(x);
    return x;
}

static Task *rot_left(Task *x)
{
    Task *y = x->right;
    x->right = y->left;
    y->left = x;
    update(x);
    update(y);
    return y;
}

static Task *rebalance(Task *t)
{
    update(t);
    int b = height(t->left) - height(t->right);
    if (b > 1)
    {
        if (height(t->left->left) < height(t->left->right))
            t->left = rot_left(t->left);
        return rot_right(t);
    }
    if (b < -1)
    {
        if (height(t->right->right) < height(t->right->left))
            t->right = rot_right(t->right);
        return rot_left(t);
    }
    return t;
}

static Task *insert(Task *root, Task *n)
{
    if (!root)
    {
        n->left = n->right = NULL;
        n->height = 1;
        return n;
    }
    if (n->vruntime < root->vruntime)
        root->left = insert(root->left, n);
    else
        root->right = insert(root->right, n);
    return rebalance(root);
}

static Task *remove_min(Task *root, Task **out)       /* detach the leftmost node */
{
    if (!root->left)
    {
        *out = root;
        return root->right;
    }
    root->left = remove_min(root->left, out);
    return rebalance(root);
}

int main(void)
{
    Task tasks[3] = {
        {0, 1024, "editor", 0, NULL, NULL},           /* nice 0 */
        {0, 1024, "compiler", 0, NULL, NULL},         /* nice 0 */
        {0, 512, "backup", 0, NULL, NULL},            /* lower weight: about half the CPU */
    };
    Task *root = NULL;
    for (int i = 0; i < 3; i++)
        root = insert(root, &tasks[i]);

    int runs[3] = {0};
    const int slice = 10;
    for (int tick = 0; tick < 40; tick++)
    {
        Task *t;
        root = remove_min(root, &t);                  /* the leftmost task has had the least */
        if (tick < 6)
            printf("tick %2d: run %-8s vruntime %ld\n", tick, t->name, t->vruntime);
        runs[t - tasks]++;
        t->vruntime += (long)slice * 1024 / t->weight;
        root = insert(root, t);
    }
    printf("runs in 40 ticks: editor=%d compiler=%d backup=%d\n", runs[0], runs[1], runs[2]);
    return 0;
}
