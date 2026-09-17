#include <stdio.h>
#include <stdlib.h>

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

// This is the trie's signature use case - autocomplete/spell-check only
// need to know "does any word start with this prefix", which is just
// walking the prefix's path and checking it exists, without needing
// isEndOfWord to be set on the final node.
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
    insert(root, "care");
    insert(root, "dog");

    printf("startsWith(ca) = %d\n", startsWith(root, "ca"));
    printf("startsWith(car) = %d\n", startsWith(root, "car"));
    printf("startsWith(cars) = %d\n", startsWith(root, "cars"));
    printf("startsWith(do) = %d\n", startsWith(root, "do"));
    printf("startsWith(cat) = %d\n", startsWith(root, "cat"));

    freeTrie(root);
    return 0;
}
