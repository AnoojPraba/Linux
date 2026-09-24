#include <iostream>
#include <type_traits>
#include <string>

// Expression SFINAE via the void_t idiom: has_serialize<T>::value is true
// only if "std::declval<T>().serialize()" is a well-formed expression.
// std::void_t<...> maps any well-formed set of types to void; if the
// expression inside decltype() is ill-formed for a given T, substitution of
// the specialization below fails (SFINAE), and the compiler falls back to
// the primary template's std::false_type.

using namespace std;

template <typename, typename = void_t<>>
struct has_serialize : false_type
{
};

template <typename T>
struct has_serialize<T, void_t<decltype(declval<T>().serialize())>>
    : true_type
{
};

// C++14+ convenience shorthand, matching the _v suffix style used for other
// traits (see 05_typeTraitsBasics.cpp).
template <typename T>
inline constexpr bool has_serialize_v = has_serialize<T>::value;

class Serializable
{
    public:
        // Trivial method with a body, so a well-formed serialize() call exists.
        string serialize() const { return "Serializable{}"; }
};

class NotSerializable
{
    public:
        // No serialize() member at all.
        int value = 0;
};

static_assert(has_serialize_v<Serializable>, "Serializable should have serialize()");
static_assert(!has_serialize_v<NotSerializable>, "NotSerializable should not have serialize()");

/*****************************************************************************
 * Name: describeSerializability
 *
 * Description:
 *         Prints, at compile time via if constexpr, whether T supports
 *         serialize().
 *
 * Returns:
 *         None.
 *****************************************************************************/
template <typename T>
void describeSerializability()
{
    if constexpr (has_serialize_v<T>)
    {
        cout << "T has serialize()\n";
    }
    else
    {
        cout << "T does NOT have serialize()\n";
    }
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Exercises has_serialize<T> against a type with and without a
 *         serialize() member; the interesting checks already happened at
 *         compile time via the static_assert lines above.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    describeSerializability<Serializable>();
    describeSerializability<NotSerializable>();
    return 0;
}
