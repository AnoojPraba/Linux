#include <stddef.h>
#include <stdio.h>

// container_of: recover the enclosing struct from a pointer to one of its
// members. Subtract the member's offset from the member's address.
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

// Intrusive doubly-linked list node (Linux kernel list_head style). The node
// lives INSIDE the user's struct, so one object can sit on several lists and
// insert/remove need zero allocation.
struct list_node
{
    struct list_node *prev, *next;
};

#define LIST_INIT(name) { &(name), &(name) }

static void list_add_tail(struct list_node *head, struct list_node *n)
{
    n->prev = head->prev;
    n->next = head;
    head->prev->next = n;
    head->prev = n;
}

static void list_del(struct list_node *n)
{
    n->prev->next = n->next;
    n->next->prev = n->prev;
    n->prev = n->next = n;
}

struct task
{
    int id;
    const char *name;
    struct list_node run_link;   // membership in the run queue
    struct list_node all_link;   // membership in the all-tasks list
};

int main(void)
{
    struct list_node runq = LIST_INIT(runq);
    struct list_node all = LIST_INIT(all);

    struct task t[3] = {
        {1, "init", {0}, {0}},
        {2, "sshd", {0}, {0}},
        {3, "cron", {0}, {0}},
    };

    for (int i = 0; i < 3; i++)
    {
        list_add_tail(&all, &t[i].all_link);
        list_add_tail(&runq, &t[i].run_link);
    }

    list_del(&t[1].run_link);   // sshd blocks: off the run queue, still in 'all'

    printf("run queue:\n");
    for (struct list_node *p = runq.next; p != &runq; p = p->next)
    {
        struct task *k = container_of(p, struct task, run_link);
        printf("  %d %s\n", k->id, k->name);
    }

    printf("all tasks:\n");
    for (struct list_node *p = all.next; p != &all; p = p->next)
    {
        struct task *k = container_of(p, struct task, all_link);
        printf("  %d %s\n", k->id, k->name);
    }

    printf("offsetof(run_link)=%zu offsetof(all_link)=%zu\n",
           offsetof(struct task, run_link), offsetof(struct task, all_link));
    return 0;
}
