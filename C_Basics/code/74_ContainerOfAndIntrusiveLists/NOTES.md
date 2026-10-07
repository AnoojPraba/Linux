# container_of, offsetof and Intrusive Lists

- `offsetof(type, member)` (`<stddef.h>`) = byte offset of a member from the
  start of the struct. `container_of` subtracts it from a member pointer to get
  back the enclosing object - pure pointer arithmetic, no extra storage.
- Intrusive list: the link node is embedded in the payload struct. Versus a
  "container" list (node holds `void *data`):
  - no per-element allocation, one fewer pointer chase (better cache behavior);
  - one object can be on many lists (run queue + all-tasks list) via several
    embedded nodes;
  - removal is O(1) given just the object - no search - and cannot fail on OOM;
  - cost: object lifetime is the caller's problem; type safety is by
    convention (`container_of` with the wrong member name is a silent bug).
- Linux kernel `list_head`, `hlist_head`, `rb_node` and BSD `<sys/queue.h>`
  (`TAILQ`, `LIST`) are all this pattern.
- Circular list with a sentinel head means no NULL checks in add/del.
- Real kernel `container_of` adds a `static_assert`/`typeof` check that `ptr`
  really points at `member`'s type; the minimal macro here does not.
- Flexible array member (`T data[];`, C99): header + payload in one
  allocation. `sizeof` excludes it; allocate `sizeof *p + n * sizeof elem`.
  Only legal as the LAST member of a struct with at least one other member;
  such a struct cannot be an array element or a member of another struct (GNU
  allows the latter as an extension).
- Interview gotchas:
  - "How does the kernel get from a list node back to the task?" -> `container_of`.
  - `data[0]` / `data[1]` tricks are pre-C99; use `[]`.
  - Alignment of the trailing array follows its element type; padding may sit
    between the header and `data`.

## Senior interviewer Q&A
**Q: How does `container_of` work, and what can go wrong?**
A: `(T *)((char *)ptr - offsetof(T, member))`. Casting to `char *` makes the
subtraction byte-granular. It silently breaks if `ptr` is not really the
address of that member (wrong member name, pointer to a copy, node not
embedded), and the compiler cannot tell. The kernel version adds a
type-compatibility check with `static_assert`/`__same_type`.
*Follow-up: why not put the node first and just cast?* Works only for one list
per object and breaks if someone reorders fields; `container_of` is robust to
layout and supports multiple embedded nodes.

**Q: Intrusive vs non-intrusive list - when do you choose which?**
A: Intrusive when objects are long-lived, on several lists, or you need O(1)
removal from a handle and no allocation on the hot/OOM-sensitive path (kernel,
schedulers, network stacks). Non-intrusive when the element type is not yours,
or ownership/lifetime should be decoupled from membership.
*Follow-up: ownership?* The list does not own objects; deleting an object that
is still linked leaves dangling `prev`/`next` in neighbors - unlink first, and
poison the pointers in debug builds (kernel `LIST_POISON1/2`).

**Q: Why does a circular list with a sentinel simplify code?**
A: No NULL checks: an empty list is `head->next == head`, and insert/delete
are the same four assignments at any position.

**Q: How do you make this list thread-safe?**
A: Lock around mutations (a spinlock or mutex per list); for read-mostly
lists, RCU variants (`list_add_rcu`) publish with a release store and readers
traverse lock-free. Lock-free doubly linked lists are hard (two pointers must
change atomically) - prefer singly linked CAS stacks/queues if lock-free is
required.

**Q: What is a flexible array member and how does it differ from `data[1]`?**
A: C99 `T data[];` as the last member: not counted in `sizeof`, one allocation
for header+payload. `data[1]` includes a phantom element in `sizeof` and
indexing beyond it was formally UB. Cannot be used in arrays of such structs.
*Follow-up: how do you allocate?* `malloc(sizeof *p + n * sizeof p->data[0])`;
use `offsetof(struct, data) + n*size` if you want to avoid trailing padding.
Check the multiplication for overflow (`n > (SIZE_MAX - hdr) / size`).

**Q: Size a hash-table node with an intrusive `hlist` - why does the kernel have `hlist` with a head of one pointer?**
A: A hash table has many buckets; a head with just `first` halves the bucket
array memory vs a two-pointer `list_head`. Nodes use `pprev` (pointer to the
previous `next`) so deletion needs no head knowledge.
