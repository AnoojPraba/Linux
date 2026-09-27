#include <iostream>
#include <string>

using namespace std;

#define SHORT_STRING "short"
#define LONG_STRING "this string is deliberately much longer than any small buffer"

/*****************************************************************************
 * Name: describeStorage
 *
 * Description:
 *         Prints whether a string's character data appears to live inside
 *         the string object itself (small string optimization / SSO) or in
 *         a separate heap allocation, by comparing the data pointer's
 *         address against the string object's own address and size. This is
 *         a libstdc++-specific implementation detail -- the C++ standard
 *         does not guarantee SSO or any particular std::string layout at
 *         all, though the general small-buffer-optimization concept applies
 *         broadly across implementations, just with different thresholds.
 *
 * Inputs:
 *         label : name to print alongside the result.
 *         value : the string to inspect.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void describeStorage(const string &label, const string &value)
{
    const char *dataPointer = value.data();
    const char *objectStart = reinterpret_cast<const char *>(&value);
    const char *objectEnd = objectStart + sizeof(string);

    bool pointsInsideObject = ((dataPointer >= objectStart) && (dataPointer < objectEnd));

    cout << label << ": length=" << value.size()
         << ", data pointer is " << (pointsInsideObject ? "INSIDE" : "OUTSIDE")
         << " the string object (" << (pointsInsideObject ? "SSO" : "heap allocation") << ")\n";
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Compares sizeof(std::string) (constant regardless of content)
 *         against the SSO behavior of a short and a long string under
 *         libstdc++.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    string shortString(SHORT_STRING);
    string longString(LONG_STRING);

    // sizeof(std::string) is fixed at compile time -- it does not grow or
    // shrink based on what the string currently holds.
    cout << "sizeof(std::string) = " << sizeof(string) << " bytes (constant)\n";

    describeStorage("shortString", shortString);
    describeStorage("longString", longString);
    return 0;
}
