#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <string>

// unordered_set/unordered_map are the STL's hash table types. Unlike a
// from-scratch hash table, bucket count, load factor, and rehashing are all
// handled automatically: the container tracks load_factor() and transparently
// rehashes into a larger bucket array once it exceeds max_load_factor(),
// rather than the caller having to notice "too full" and manually resize.

using namespace std;

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates unordered_set/unordered_map insert, find, and erase,
 *         plus inspecting load factor and bucket count.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    unordered_set<string> seenNames;
    seenNames.insert("alice");
    seenNames.insert("bob");
    seenNames.insert("alice"); // duplicate, ignored

    cout << "unordered_set size: " << seenNames.size() << "\n";
    cout << "contains 'bob': " << (seenNames.find("bob") != seenNames.end() ? "true" : "false")
         << "\n";

    seenNames.erase("alice");
    cout << "contains 'alice' after erase: "
         << (seenNames.find("alice") != seenNames.end() ? "true" : "false") << "\n";

    unordered_map<string, int> wordCounts;
    wordCounts.insert({"apple", 1});
    wordCounts["banana"] = 2;
    wordCounts["apple"] = wordCounts["apple"] + 1; // find-or-insert then update

    cout << "unordered_map contents:\n";
    for (const auto &[word, count] : wordCounts)
    {
        cout << "  " << word << " -> " << count << "\n";
    }

    auto found = wordCounts.find("banana");
    if (found != wordCounts.end())
    {
        cout << "found 'banana' -> " << found->second << "\n";
    }

    wordCounts.erase("banana");
    cout << "contains 'banana' after erase: "
         << (wordCounts.find("banana") != wordCounts.end() ? "true" : "false") << "\n";

    // Rehashing is automatic: adding elements can grow bucket_count() once
    // load_factor() would exceed max_load_factor(), all without caller
    // involvement.
    cout << "bucket count: " << wordCounts.bucket_count() << ", load factor: "
         << wordCounts.load_factor() << ", max load factor: " << wordCounts.max_load_factor()
         << "\n";

    return 0;
}
