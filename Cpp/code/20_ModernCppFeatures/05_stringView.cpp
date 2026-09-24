#include <iostream>
#include <string_view>
#include <string>

using namespace std;

/*****************************************************************************
 * Name: startsWith
 *
 * Description:
 *         Checks whether text begins with prefix, taking both as
 *         string_view so no copy is made regardless of whether the caller
 *         passes a std::string, a C string literal, or another view.
 *
 * Inputs:
 *         text   : the text to inspect.
 *         prefix : the prefix to look for.
 *
 * Returns:
 *         true if text begins with prefix, false otherwise.
 *****************************************************************************/
bool startsWith(string_view text, string_view prefix)
{
    return text.substr(0, prefix.size()) == prefix;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates std::string_view as a non-owning view over existing
 *         character data, avoiding allocations that std::string parameters
 *         would require for temporaries and literals.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    string owned = "hello, world";
    string_view view = owned;

    cout << "view = " << view << "\n";
    cout << "startsWith(owned, \"hello\") = " << startsWith(owned, "hello") << "\n";
    cout << "startsWith(\"literal text\", \"lit\") = " << startsWith("literal text", "lit") << "\n";

    // note: string_view can dangle if the underlying storage is destroyed or
    // reallocated while the view is still in use (e.g. viewing a temporary).
    string_view firstWord = view.substr(0, view.find(','));
    cout << "firstWord = " << firstWord << "\n";

    return 0;
}
