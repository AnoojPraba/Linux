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

// Each node has one child slot per possible next character, instead of a
// BST's two (left/right) - insertion walks one character at a time,
// creating a node only where the path doesn't already exist.
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

// A full word match requires both reaching the last character's node *and*
// isEndOfWord being set - without that flag, "cat" would also report a
// match for a search of "ca" if "cat" was inserted, since the path exists.
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

    insert(root, "cat");
    insert(root, "car");
    insert(root, "dog");

    printf("search(cat) = %d\n", search(root, "cat"));
    printf("search(ca) = %d\n", search(root, "ca"));
    printf("search(car) = %d\n", search(root, "car"));
    printf("search(dogs) = %d\n", search(root, "dogs"));

    freeTrie(root);
    return 0;
}
