#include <iostream>

using namespace std;

#define IMPLICIT_LENGTH_CM 42

// WITHOUT explicit: a single-argument constructor is usable as an implicit
// conversion. Passing an int where a Distance was expected silently compiles,
// even though that is probably not what the caller meant.
class ImplicitDistance
{
    private:
        int centimeters;

    public:
        // Not explicit: allows implicit int -> ImplicitDistance conversion.
        ImplicitDistance(int cm) : centimeters(cm) {}

        // Trivial getter.
        int getCentimeters() const { return centimeters; }
};

/*****************************************************************************
 * Name: printImplicitDistance
 *
 * Description:
 *         Prints a distance in centimeters.
 *
 * Inputs:
 *         distance : the distance to print.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void printImplicitDistance(ImplicitDistance distance)
{
    cout << distance.getCentimeters() << " cm\n";
}

// WITH explicit: the same single-argument constructor now requires an
// explicit, intentional conversion. Passing a bare int now fails to compile,
// catching the same mistake at build time instead of letting it slip through.
class ExplicitDistance
{
    private:
        int centimeters;

    public:
        // explicit: blocks implicit int -> ExplicitDistance conversion.
        explicit ExplicitDistance(int cm) : centimeters(cm) {}

        // Trivial getter.
        int getCentimeters() const { return centimeters; }
};

/*****************************************************************************
 * Name: printExplicitDistance
 *
 * Description:
 *         Prints a distance in centimeters.
 *
 * Inputs:
 *         distance : the distance to print.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void printExplicitDistance(ExplicitDistance distance)
{
    cout << distance.getCentimeters() << " cm\n";
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Shows the accidental implicit conversion compiling for
 *         ImplicitDistance, and the equivalent call for ExplicitDistance that
 *         requires an explicit conversion (the commented-out line below would
 *         fail to compile: "printExplicitDistance(IMPLICIT_LENGTH_CM);").
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    // Accidental implicit conversion -- compiles, probably not intended.
    printImplicitDistance(IMPLICIT_LENGTH_CM);

    // Same call would fail to compile with the explicit constructor:
    // printExplicitDistance(IMPLICIT_LENGTH_CM); // error: no implicit conversion
    printExplicitDistance(ExplicitDistance(IMPLICIT_LENGTH_CM));
    return 0;
}
