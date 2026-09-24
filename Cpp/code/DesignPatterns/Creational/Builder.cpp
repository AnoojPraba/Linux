#include <iostream>
#include <string>

using namespace std;

// Builder: assemble a Pizza step by step.
class Pizza
{
public:
    string size;
    bool cheese = false;
    bool pepperoni = false;
    bool mushrooms = false;

    /*****************************************************************************
     * Name: describe
     *
     * Description:
     *         Prints a human-readable summary of the pizza's toppings.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void describe() const
    {
        cout << size << " pizza with:";
        if (cheese)
        {
            cout << " cheese";
        }
        if (pepperoni)
        {
            cout << " pepperoni";
        }
        if (mushrooms)
        {
            cout << " mushrooms";
        }
        cout << endl;
    }
};

class PizzaBuilder
{
public:
    // returns builder for chaining
    PizzaBuilder& setSize(const string& newSize)
    {
        pizza.size = newSize;
        return *this;
    }

    // returns builder for chaining
    PizzaBuilder& addCheese()
    {
        pizza.cheese = true;
        return *this;
    }

    // returns builder for chaining
    PizzaBuilder& addPepperoni()
    {
        pizza.pepperoni = true;
        return *this;
    }

    // returns builder for chaining
    PizzaBuilder& addMushrooms()
    {
        pizza.mushrooms = true;
        return *this;
    }

    // returns the finished product
    Pizza build() const
    {
        return pizza;
    }

private:
    Pizza pizza;
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Uses PizzaBuilder to assemble two different pizzas step by step.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    Pizza veggie = PizzaBuilder().setSize("Medium").addCheese().addMushrooms().build();
    Pizza meatLovers = PizzaBuilder().setSize("Large").addCheese().addPepperoni().build();
    veggie.describe();
    meatLovers.describe();
    return 0;
}
