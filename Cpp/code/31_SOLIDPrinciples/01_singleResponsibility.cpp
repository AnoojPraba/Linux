#include <iostream>
#include <string>

using namespace std;

// VIOLATION: Report does data computation, formatting/printing, AND file saving.
// Three separate reasons to change are bundled into one class.
class ReportBad
{
public:
    // trivial ctor
    explicit ReportBad(int value) : total(value) {}

    // trivial getter
    int compute() const
    {
        return total * 2;
    }

    /*****************************************************************************
     * Name: printReport
     *
     * Description:
     *         Formats and prints the computed report value to stdout.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void printReport() const
    {
        cout << "Report total: " << compute() << endl;
    }

    /*****************************************************************************
     * Name: saveToFile
     *
     * Description:
     *         Pretends to persist the report to disk (stubbed for this example).
     *
     * Inputs:
     *         filename : path to save the report to.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void saveToFile(const string &filename) const
    {
        cout << "Saving report to " << filename << endl;
    }

private:
    int total;
};

// FIX: each class has exactly one reason to change.
class ReportCalculator
{
public:
    // trivial ctor
    explicit ReportCalculator(int value) : total(value) {}

    // trivial getter
    int compute() const
    {
        return total * 2;
    }

private:
    int total;
};

class ReportPrinter
{
public:
    /*****************************************************************************
     * Name: print
     *
     * Description:
     *         Prints an already-computed report value to stdout.
     *
     * Inputs:
     *         value : the computed report value to display.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void print(int value) const
    {
        cout << "Report total: " << value << endl;
    }
};

class ReportSaver
{
public:
    /*****************************************************************************
     * Name: save
     *
     * Description:
     *         Pretends to persist a computed report value to disk.
     *
     * Inputs:
     *         filename : path to save the report to.
     *         value    : the computed report value to persist.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void save(const string &filename, int value) const
    {
        cout << "Saving report to " << filename << ": " << value << endl;
    }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates the SRP violation followed by the fixed, split design.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    ReportBad bad(21);
    bad.printReport();
    bad.saveToFile("bad_report.txt");

    ReportCalculator calculator(21);
    ReportPrinter printer;
    ReportSaver saver;
    int value = calculator.compute();
    printer.print(value);
    saver.save("good_report.txt", value);

    return 0;
}
