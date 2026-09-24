#include <iostream>
#include <memory>
#include <string>

using namespace std;

// Public-facing class. Its header (here, this class declaration) exposes no
// private data members or private helper types - only a pointer to a forward
// declared Impl struct. Callers depending on Widget never see Impl's layout,
// so changing Impl's members does not force callers to recompile (compile-time
// firewall) and does not change Widget's own size (ABI stability).
class Widget
{
public:
    Widget(const string &name, int value);
    ~Widget();

    Widget(const Widget &other);
    Widget &operator=(const Widget &other);
    Widget(Widget &&other) noexcept;
    Widget &operator=(Widget &&other) noexcept;

    void print() const;
    void bump();

private:
    struct Impl;
    unique_ptr<Impl> pImpl;
};

// Impl is only fully defined here, in the .cpp file. It can freely hold
// private members, include heavy headers, or change shape without touching
// Widget's public header at all.
struct Widget::Impl
{
    string name;
    int value;

    Impl(const string &nameIn, int valueIn) : name(nameIn), value(valueIn)
    {
    }
};

/*****************************************************************************
 * Name: Widget::Widget
 *
 * Description:
 *         Constructs the Widget by heap-allocating its Impl.
 *
 * Inputs:
 *         name  : label stored in the implementation.
 *         value : integer payload stored in the implementation.
 *
 * Returns:
 *         None.
 *****************************************************************************/
Widget::Widget(const string &name, int value) : pImpl(make_unique<Impl>(name, value))
{
}

// unique_ptr's deleter needs the complete Impl type, so the destructor must be
// defined here (where Impl is complete), not implicitly in the header.
Widget::~Widget() = default;

/*****************************************************************************
 * Name: Widget::Widget
 *
 * Description:
 *         Copy constructs by deep-copying the pointee Impl.
 *
 * Inputs:
 *         other : source Widget to copy from.
 *
 * Returns:
 *         None.
 *****************************************************************************/
Widget::Widget(const Widget &other) : pImpl(make_unique<Impl>(*other.pImpl))
{
}

/*****************************************************************************
 * Name: Widget::operator=
 *
 * Description:
 *         Copy-assigns by deep-copying the pointee Impl.
 *
 * Inputs:
 *         other : source Widget to copy from.
 *
 * Returns:
 *         Reference to *this.
 *****************************************************************************/
Widget &Widget::operator=(const Widget &other)
{
    if (this != &other)
    {
        pImpl = make_unique<Impl>(*other.pImpl);
    }
    return *this;
}

// Move operations can just move the pointer itself - no Impl copy needed.
Widget::Widget(Widget &&other) noexcept = default;
Widget &Widget::operator=(Widget &&other) noexcept = default;

/*****************************************************************************
 * Name: Widget::print
 *
 * Description:
 *         Prints the hidden implementation state.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void Widget::print() const
{
    cout << "Widget(name=" << pImpl->name << ", value=" << pImpl->value << ")\n";
}

/*****************************************************************************
 * Name: Widget::bump
 *
 * Description:
 *         Increments the hidden implementation's value by one.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void Widget::bump()
{
    pImpl->value = pImpl->value + 1;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Exercises Widget's public API without ever seeing Impl's layout.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    Widget original("first", 1);
    original.print();
    original.bump();
    original.print();

    Widget copy = original;
    copy.bump();
    cout << "after copy + bump on copy:\n";
    original.print();
    copy.print();

    Widget moved = std::move(copy);
    moved.print();

    return 0;
}
