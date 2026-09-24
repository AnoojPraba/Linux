#include <iostream>

using namespace std;

// Demonstrates the diamond problem with multiple inheritance, and its fix
// using virtual inheritance.
class Device
{
    protected:
        int id;

    public:
        // Trivial constructor with initializer list.
        Device(int deviceId) : id(deviceId) {}

        // Trivial getter.
        int getId() const { return id; }
};

// Both Scanner and Printer inherit virtually from Device so that
// MultiFunctionPrinter has only one shared Device sub-object.
class Scanner : virtual public Device
{
    public:
        // Trivial constructor with initializer list, chaining to Device.
        Scanner(int deviceId) : Device(deviceId) {}

        /*****************************************************************************
         * Name: scan
         *
         * Description:
         *         Prints a scan message identifying the shared Device id.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void scan() const
        {
            cout << "Scanner (device " << id << ") scanning\n";
        }
};

class Printer : virtual public Device
{
    public:
        // Trivial constructor with initializer list, chaining to Device.
        Printer(int deviceId) : Device(deviceId) {}

        /*****************************************************************************
         * Name: print
         *
         * Description:
         *         Prints a print message identifying the shared Device id.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void print() const
        {
            cout << "Printer (device " << id << ") printing\n";
        }
};

class MultiFunctionPrinter : public Scanner, public Printer
{
    public:
        /*****************************************************************************
         * Name: MultiFunctionPrinter
         *
         * Description:
         *         Constructs a MultiFunctionPrinter, initializing the single
         *         shared virtual Device base directly (required because it is a
         *         virtual base), then Scanner and Printer.
         *
         * Inputs:
         *         deviceId : the id shared by the single virtual Device base.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        MultiFunctionPrinter(int deviceId)
            : Device(deviceId), Scanner(deviceId), Printer(deviceId)
        {
        }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Creates a MultiFunctionPrinter and shows both inherited paths
 *         (Scanner and Printer) share a single Device id, thanks to virtual
 *         inheritance resolving the diamond problem.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    MultiFunctionPrinter mfp(7);
    mfp.scan();
    mfp.print();
    cout << "shared device id via mfp.getId(): " << mfp.getId() << "\n";
    return 0;
}
