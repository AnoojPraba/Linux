#include <iostream>
#include <string>

using namespace std;

#define MIN_DIVISOR 0

// Custom exception class demonstrating deriving from exception.
class DivideByZeroException : public exception
{
    private:
        string message;

    public:
        // Trivial constructor with initializer list.
        DivideByZeroException(const string &msg) : message(msg) {}

        // Trivial override of what().
        const char *what() const noexcept override { return message.c_str(); }
};

// RAII helper used to demonstrate stack unwinding: its destructor runs even
// when an exception propagates past it.
class UnwindTracer
{
    private:
        string label;

    public:
        // Trivial constructor with initializer list.
        UnwindTracer(const string &tracerLabel) : label(tracerLabel) {}

        // Trivial destructor that proves unwinding occurred.
        ~UnwindTracer() { cout << "unwinding: " << label << " destroyed\n"; }
};

/*****************************************************************************
 * Name: divide
 *
 * Description:
 *         Divides numerator by denominator, throwing a DivideByZeroException
 *         if denominator is zero.
 *
 * Inputs:
 *         numerator   : the value to divide.
 *         denominator : the value to divide by.
 *
 * Returns:
 *         The quotient of numerator and denominator.
 *****************************************************************************/
int divide(int numerator, int denominator)
{
    UnwindTracer tracer("divide's local tracer");
    if (denominator == MIN_DIVISOR)
    {
        throw DivideByZeroException("attempted to divide by zero");
    }
    return numerator / denominator;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Calls divide() with a valid and an invalid denominator, catching
 *         the custom exception and observing stack unwinding output.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    try
    {
        cout << "10 / 2 = " << divide(10, 2) << "\n";
        cout << "10 / 0 = " << divide(10, 0) << "\n";
    }
    catch (const DivideByZeroException &e)
    {
        cout << "caught exception: " << e.what() << "\n";
    }
    catch (const exception &e)
    {
        cout << "caught unexpected exception: " << e.what() << "\n";
    }
    return 0;
}
