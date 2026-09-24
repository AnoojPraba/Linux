#include <iostream>
#include <type_traits>
#include <vector>

#define ELEMENT_COUNT 5
#define RESERVED_CAPACITY 1

using namespace std;

// Move constructor is NOT noexcept. On reallocation, vector<NoisyThrowingMove> cannot
// safely use it: if a move threw partway through, the vector could be left with some
// elements moved-from and some not, breaking the strong exception guarantee for
// push_back/reallocation. So the standard requires falling back to copying instead.
class NoisyThrowingMove
{
public:
    NoisyThrowingMove()
    {
    }

    NoisyThrowingMove(const NoisyThrowingMove &)
    {
        cout << "  NoisyThrowingMove copy constructed (reallocation copied)\n";
    }

    NoisyThrowingMove(NoisyThrowingMove &&)
    {
        cout << "  NoisyThrowingMove move constructed\n";
    }
};

// Move constructor IS noexcept. On reallocation, vector can safely move elements into
// the new buffer - if a move can't throw, there's no risk of leaving the vector in a
// half-moved, exception-unsafe state, so the standard permits (and libstdc++/libc++
// perform) moving instead of copying.
class NoisyNoexceptMove
{
public:
    NoisyNoexceptMove()
    {
    }

    NoisyNoexceptMove(const NoisyNoexceptMove &)
    {
        cout << "  NoisyNoexceptMove copy constructed\n";
    }

    NoisyNoexceptMove(NoisyNoexceptMove &&) noexcept
    {
        cout << "  NoisyNoexceptMove move constructed (reallocation moved)\n";
    }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Statically confirms each type's move-noexcept-ness, then pushes
 *         enough elements to force a reallocation for both a throwing-move
 *         type and a noexcept-move type, printing which constructor
 *         push_back's internal reallocation actually chooses for each.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    static_assert(!is_nothrow_move_constructible<NoisyThrowingMove>::value,
                  "NoisyThrowingMove's move ctor is expected to be throwing");
    static_assert(is_nothrow_move_constructible<NoisyNoexceptMove>::value,
                  "NoisyNoexceptMove's move ctor is expected to be noexcept");

    cout << "-- vector<NoisyThrowingMove>: reallocation falls back to copy --\n";
    vector<NoisyThrowingMove> throwingMoveVec;
    throwingMoveVec.reserve(RESERVED_CAPACITY);
    for (int i = 0; i < ELEMENT_COUNT; i = i + 1)
    {
        throwingMoveVec.emplace_back();
    }

    cout << "-- vector<NoisyNoexceptMove>: reallocation moves elements --\n";
    vector<NoisyNoexceptMove> noexceptMoveVec;
    noexceptMoveVec.reserve(RESERVED_CAPACITY);
    for (int i = 0; i < ELEMENT_COUNT; i = i + 1)
    {
        noexceptMoveVec.emplace_back();
    }

    return 0;
}
