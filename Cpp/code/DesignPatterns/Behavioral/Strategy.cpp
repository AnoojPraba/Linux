#include <iostream>
#include <memory>

// Strategy: interchangeable payment algorithms chosen at runtime.
class PaymentStrategy
{
public:
    virtual ~PaymentStrategy() = default;
    virtual void pay(double amount) const = 0;
};

class CreditCardPayment : public PaymentStrategy
{
public:
    // trivial override
    void pay(double amount) const override
    {
        std::cout << "Paid " << amount << " using credit card" << std::endl;
    }
};

class PayPalPayment : public PaymentStrategy
{
public:
    // trivial override
    void pay(double amount) const override
    {
        std::cout << "Paid " << amount << " using PayPal" << std::endl;
    }
};

class ShoppingCart
{
public:
    // trivial setter
    void setPaymentStrategy(std::unique_ptr<PaymentStrategy> strategy)
    {
        paymentStrategy = std::move(strategy);
    }

    /*****************************************************************************
     * Name: checkout
     *
     * Description:
     *         Pays the given amount using whichever payment strategy is
     *         currently configured.
     *
     * Inputs:
     *         amount : the total to pay.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void checkout(double amount) const
    {
        paymentStrategy->pay(amount);
    }

private:
    std::unique_ptr<PaymentStrategy> paymentStrategy;
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Checks out the same cart with two different payment strategies.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    ShoppingCart cart;
    cart.setPaymentStrategy(std::make_unique<CreditCardPayment>());
    cart.checkout(49.99);

    cart.setPaymentStrategy(std::make_unique<PayPalPayment>());
    cart.checkout(19.99);
    return 0;
}
