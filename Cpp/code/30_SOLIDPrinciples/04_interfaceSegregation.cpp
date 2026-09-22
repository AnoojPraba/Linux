#include <iostream>
#include <stdexcept>

// VIOLATION: one fat interface forces every implementer to provide (or stub
// out) methods it doesn't logically support.
class MultiFunctionDeviceBad
{
public:
    virtual ~MultiFunctionDeviceBad() = default;
    virtual void print() = 0;
    virtual void scan() = 0;
    virtual void fax() = 0;
};

class OldPrinterBad : public MultiFunctionDeviceBad
{
public:
    // trivial override
    void print() override
    {
        std::cout << "Printing document" << std::endl;
    }

    // stubbed out: this printer cannot scan
    void scan() override
    {
        throw std::logic_error("OldPrinterBad cannot scan");
    }

    // stubbed out: this printer cannot fax
    void fax() override
    {
        throw std::logic_error("OldPrinterBad cannot fax");
    }
};

// FIX: split into small, focused interfaces so implementers only take on
// the capabilities they actually support.
class Printable
{
public:
    virtual ~Printable() = default;
    virtual void print() = 0;
};

class Scannable
{
public:
    virtual ~Scannable() = default;
    virtual void scan() = 0;
};

class Faxable
{
public:
    virtual ~Faxable() = default;
    virtual void fax() = 0;
};

class SimplePrinter : public Printable
{
public:
    // trivial override
    void print() override
    {
        std::cout << "Printing document" << std::endl;
    }
};

class AllInOnePrinter : public Printable, public Scannable, public Faxable
{
public:
    // trivial override
    void print() override
    {
        std::cout << "Printing document" << std::endl;
    }

    // trivial override
    void scan() override
    {
        std::cout << "Scanning document" << std::endl;
    }

    // trivial override
    void fax() override
    {
        std::cout << "Faxing document" << std::endl;
    }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates the ISP violation (a device forced to stub out
 *         unsupported operations) followed by the fixed design using small,
 *         focused interfaces implemented only where they make sense.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    OldPrinterBad badDevice;
    badDevice.print();
    try
    {
        badDevice.scan();
    }
    catch (const std::logic_error &e)
    {
        std::cout << "Caught expected error: " << e.what() << std::endl;
    }

    SimplePrinter simple;
    simple.print();

    AllInOnePrinter allInOne;
    allInOne.print();
    allInOne.scan();
    allInOne.fax();

    return 0;
}
