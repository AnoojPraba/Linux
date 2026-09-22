#include <iostream>

#define PADDING_VALUE 42

// Instruments every special member function so construction/copy/move activity is
// visible on stdout.
class Traced
{
public:
    Traced() : payload(PADDING_VALUE)
    {
        std::cout << "  Traced() default construct\n";
    }

    Traced(const Traced &other) : payload(other.payload)
    {
        std::cout << "  Traced(const Traced&) copy construct\n";
    }

    Traced(Traced &&other) noexcept : payload(other.payload)
    {
        std::cout << "  Traced(Traced&&) move construct\n";
    }

    Traced &operator=(const Traced &other)
    {
        payload = other.payload;
        std::cout << "  operator=(const Traced&) copy assign\n";
        return *this;
    }

    Traced &operator=(Traced &&other) noexcept
    {
        payload = other.payload;
        std::cout << "  operator=(Traced&&) move assign\n";
        return *this;
    }

    ~Traced()
    {
        std::cout << "  ~Traced() destruct\n";
    }

private:
    int payload;
};

/*****************************************************************************
 * Name: makeViaTemporary
 *
 * Description:
 *         Returns a prvalue temporary directly. Since C++17, this case
 *         (returning a prvalue of the function's return type) is mandatory
 *         copy elision - the standard guarantees no copy/move constructor is
 *         invoked at all; the object is constructed directly in the caller's
 *         storage.
 *
 * Returns:
 *         A freshly constructed Traced, guaranteed elided into the caller.
 *****************************************************************************/
Traced makeViaTemporary()
{
    return Traced();
}

/*****************************************************************************
 * Name: makeViaNamedLocal
 *
 * Description:
 *         Returns a named local variable of the return type. This is the
 *         classic NRVO (named return value optimization) case: the compiler
 *         is PERMITTED, but not REQUIRED by the standard, to construct
 *         "local" directly in the caller's storage and skip the move. Every
 *         mainstream compiler (including this repo's GCC) performs NRVO here
 *         at typical optimization levels, but portable code must not rely on
 *         it - a debug build or another compiler could legally still move.
 *
 * Returns:
 *         The local Traced, likely (not guaranteed) elided into the caller.
 *****************************************************************************/
Traced makeViaNamedLocal()
{
    Traced local;
    std::cout << "  (about to return named local)\n";
    return local;
}

/*****************************************************************************
 * Name: makeViaMovedLocal
 *
 * Description:
 *         Same as makeViaNamedLocal, but explicitly std::move's the return
 *         value. This DEFEATS NRVO: std::move casts the named local to an
 *         xvalue, so the compiler can no longer treat it as the elidable
 *         "return a local variable" pattern and must fall back to the move
 *         constructor. This is why std::move on a return statement is
 *         usually a pessimization - it turns a potential zero-copy elision
 *         into a guaranteed move.
 *
 * Returns:
 *         The local Traced, moved (elision is not possible here).
 *****************************************************************************/
Traced makeViaMovedLocal()
{
    Traced local;
    return std::move(local);
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Compares construction traces for the mandatory-elision, likely-NRVO,
 *         and move-defeats-NRVO cases side by side.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    std::cout << "-- makeViaTemporary (guaranteed elision in C++17) --\n";
    Traced fromTemporary = makeViaTemporary();

    std::cout << "-- makeViaNamedLocal (NRVO likely, not guaranteed) --\n";
    Traced fromNamedLocal = makeViaNamedLocal();

    std::cout << "-- makeViaMovedLocal (std::move defeats NRVO) --\n";
    Traced fromMovedLocal = makeViaMovedLocal();

    (void)fromTemporary;
    (void)fromNamedLocal;
    (void)fromMovedLocal;

    return 0;
}
