#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

typedef struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
} TrieNode;

TrieNode *createNode(void)
{
    TrieNode *node = malloc(sizeof(TrieNode));
    int i;

    node->isEndOfWord = 0;
    for (i = 0; i < ALPHABET_SIZE; i++)
    {
        node->children[i] = NULL;
    }
    return node;
}

void insert(TrieNode *root, const char *word)
{
    TrieNode *current = root;
    int i;

    for (i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';

        if (current->children[index] == NULL)
        {
            current->children[index] = createNode();
        }
        current = current->children[index];
    }
    current->isEndOfWord = 1;
}

int search(TrieNode *root, const char *word)
{
    TrieNode *current = root;
    int i;

    for (i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';

        if (current->children[index] == NULL)
        {
            return 0;
        }
        current = current->children[index];
    }
    return current->isEndOfWord;
}

int startsWith(TrieNode *root, const char *prefix)
{
    TrieNode *current = root;
    int i;

    for (i = 0; prefix[i] != '\0'; i++)
    {
        int index = prefix[i] - 'a';

        if (current->children[index] == NULL)
        {
            return 0;
        }
        current = current->children[index];
    }
    return 1;
}

// Returns 1 if a node has no children left, meaning it is safe for the caller
// (its parent) to unlink and free it once it is also not an end-of-word node.
int hasNoChildren(TrieNode *node)
{
    int i;

    for (i = 0; i < ALPHABET_SIZE; i++)
    {
        if (node->children[i] != NULL)
        {
            return 0;
        }
    }
    return 1;
}

// Walks down to the word's last node, clears isEndOfWord there, then unwinds
// back up the recursion pruning each node that is both childless and not the
// end of some other word - nodes still shared with another word either have
// another child set or have isEndOfWord set, so they are left untouched.
int deleteWordHelper(TrieNode *current, const char *word, int depth)
{
    int index;

    if (current == NULL)
    {
        return 0;
    }
    if (word[depth] == '\0')
    {
        if (!current->isEndOfWord)
        {
            return 0;
        }
        current->isEndOfWord = 0;
        return hasNoChildren(current);
    }

    index = word[depth] - 'a';
    if (deleteWordHelper(current->children[index], word, depth + 1))
    {
        free(current->children[index]);
        current->children[index] = NULL;
    }

    return ((!current->isEndOfWord) && hasNoChildren(current));
}

void deleteWord(TrieNode *root, const char *word)
{
    deleteWordHelper(root, word, 0);
}

void freeTrie(TrieNode *root)
{
    int i;

    if (root == NULL)
    {
        return;
    }
    for (i = 0; i < ALPHABET_SIZE; i++)
    {
        freeTrie(root->children[i]);
    }
    free(root);
}

int main()
{
    TrieNode *root = createNode();

    insert(root, "car");
    insert(root, "card");
    insert(root, "dog");

    printf("-- Initial state --\n");
    printf("search(car) = %d\n", search(root, "car"));
    printf("search(card) = %d\n", search(root, "card"));
    printf("search(dog) = %d\n", search(root, "dog"));

    printf("\n-- Deleting \"card\" (shares prefix \"car\" with another word) --\n");
    deleteWord(root, "card");
    printf("search(card) = %d (expected 0)\n", search(root, "card"));
    printf("search(car) = %d (expected 1, shared prefix survives)\n", search(root, "car"));
    printf("startsWith(car) = %d (expected 1)\n", startsWith(root, "car"));

    printf("\n-- Deleting \"dog\" (unique path, no shared prefix) --\n");
    deleteWord(root, "dog");
    printf("search(dog) = %d (expected 0)\n", search(root, "dog"));
    printf("startsWith(dog) = %d (expected 0, unique nodes pruned)\n", startsWith(root, "dog"));
    printf("startsWith(d) = %d (expected 0, unique nodes pruned)\n", startsWith(root, "d"));
    printf("search(car) = %d (expected 1, unrelated word still intact)\n", search(root, "car"));

    freeTrie(root);
    return 0;
}
