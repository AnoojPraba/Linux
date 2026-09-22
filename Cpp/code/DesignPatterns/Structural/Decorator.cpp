#include <iostream>
#include <memory>

#define MILK_COST 0.50
#define SUGAR_COST 0.20

// Decorator: add condiments to a coffee without modifying the base class.
class Coffee
{
public:
    virtual ~Coffee() = default;
    virtual double cost() const = 0;
    virtual std::string description() const = 0;
};

class PlainCoffee : public Coffee
{
public:
    // trivial override
    double cost() const override
    {
        return 1.50;
    }

    // trivial override
    std::string description() const override
    {
        return "Coffee";
    }
};

class CoffeeDecorator : public Coffee
{
public:
    // trivial constructor
    explicit CoffeeDecorator(std::unique_ptr<Coffee> wrapped) : wrapped(std::move(wrapped)) {}

protected:
    std::unique_ptr<Coffee> wrapped;
};

class MilkDecorator : public CoffeeDecorator
{
public:
    // trivial constructor
    explicit MilkDecorator(std::unique_ptr<Coffee> wrapped)
        : CoffeeDecorator(std::move(wrapped)) {}

    // trivial override
    double cost() const override
    {
        return wrapped->cost() + MILK_COST;
    }

    // trivial override
    std::string description() const override
    {
        return wrapped->description() + " + Milk";
    }
};

class SugarDecorator : public CoffeeDecorator
{
public:
    // trivial constructor
    explicit SugarDecorator(std::unique_ptr<Coffee> wrapped)
        : CoffeeDecorator(std::move(wrapped)) {}

    // trivial override
    double cost() const override
    {
        return wrapped->cost() + SUGAR_COST;
    }

    // trivial override
    std::string description() const override
    {
        return wrapped->description() + " + Sugar";
    }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Wraps a plain coffee with milk and sugar decorators and prints the
 *         resulting description and total cost.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    std::unique_ptr<Coffee> order = std::make_unique<PlainCoffee>();
    order = std::make_unique<MilkDecorator>(std::move(order));
    order = std::make_unique<SugarDecorator>(std::move(order));

    std::cout << order->description() << " costs $" << order->cost() << std::endl;
    return 0;
}
