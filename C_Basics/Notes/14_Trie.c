A trie (from "reTRIEval", usually pronounced "try") is a tree specialized for
storing strings, where each edge represents one character rather than the
whole node being a single stored value like a BST's is.

Shape:

*   The root represents the empty string.
*   Each node has up to ALPHABET_SIZE children - one slot per possible next
    character, not just two (left/right) like a BST.
*   A path from the root spells out a prefix; a node flagged isEndOfWord
    means that path is a complete word, not just a prefix of a longer one.

Why not just store strings in a hash set or sorted array?

*   Shared prefixes are stored once. "car", "care", and "card" all share
    the "car" path - only the last character(s) branch. This is what makes
    a trie memory-efficient for a dictionary of related words, and why
    lookups only cost O(length of the string), independent of how many
    other strings are stored.
*   Prefix queries are what a trie is built for. "does any word start with
    X" is a plain O(length of X) walk down the tree - a hash set can only
    answer "is this exact string present", it has no notion of "prefix of".

Two things a trie needs to distinguish, which is easy to get wrong:

*   isEndOfWord true - this path is a complete word.
*   A node existing at all - this path is at least a *prefix* of something
    stored, even if it isn't itself a complete word.

For example, after inserting "car" and "card": the "car" node has
isEndOfWord = 1 (it's a complete word) and still has a child 'd' (because
"card" continues past it). search("car") should return true;
search("ca") should return false (it's only a prefix, never inserted as a
word); startsWith("ca") should return true (something does start with it).

What tries are actually used for:

*   Autocomplete / typeahead search.
*   Spell checkers (is this a valid word / what are close matches).
*   IP routing tables (longest-prefix matching, using bits instead of
    characters as the "alphabet").

See the runnable examples: `code/28_Trie/01_insertSearch.c` (insert and
exact-word search) and `code/28_Trie/02_prefixSearch.c` (the startsWith
prefix query that's the trie's signature use case).
