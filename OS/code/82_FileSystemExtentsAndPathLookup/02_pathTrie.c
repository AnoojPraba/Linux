// Trie of path components: add and resolve /usr/bin/gcc style paths.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    char name[16];
    int inode;                      /* 0 = this component is not a file */
    struct Node *child, *sibling;   /* first child, next entry in the same directory */
} Node;

static Node *new_node(const char *name)
{
    Node *n = calloc(1, sizeof *n);
    snprintf(n->name, sizeof n->name, "%s", name);
    return n;
}

static Node *find_child(Node *dir, const char *name)
{
    for (Node *c = dir->child; c; c = c->sibling)
        if (strcmp(c->name, name) == 0)
            return c;
    return NULL;
}

static void add_path(Node *root, const char *path, int inode)
{
    char buf[64];
    snprintf(buf, sizeof buf, "%s", path);
    Node *cur = root;
    for (char *tok = strtok(buf, "/"); tok; tok = strtok(NULL, "/"))
    {
        Node *c = find_child(cur, tok);
        if (!c)
        {
            c = new_node(tok);
            c->sibling = cur->child;
            cur->child = c;
        }
        cur = c;
    }
    cur->inode = inode;
}

static int resolve(Node *root, const char *path)
{
    char buf[64];
    snprintf(buf, sizeof buf, "%s", path);
    Node *cur = root;
    for (char *tok = strtok(buf, "/"); tok; tok = strtok(NULL, "/"))
    {
        cur = find_child(cur, tok);
        if (!cur)
            return -1;              /* a component does not exist */
    }
    return cur->inode;
}

static void destroy(Node *n)
{
    if (n)
    {
        destroy(n->child);
        destroy(n->sibling);
        free(n);
    }
}

int main(void)
{
    Node *root = new_node("/");
    add_path(root, "/usr/bin/gcc", 101);
    add_path(root, "/usr/bin/ls", 102);
    add_path(root, "/usr/lib/libc.so", 103);
    add_path(root, "/etc/passwd", 104);

    const char *tests[] = {"/usr/bin/gcc", "/usr/lib/libc.so", "/etc/passwd", "/usr/bin/vim", "/var/log"};
    for (int i = 0; i < 5; i++)
    {
        int ino = resolve(root, tests[i]);
        if (ino > 0)
            printf("%-18s -> inode %d\n", tests[i], ino);
        else
            printf("%-18s -> not found\n", tests[i]);
    }
    destroy(root);
    return 0;
}
