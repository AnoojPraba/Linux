#include <iostream>
#include <memory>

using namespace std;

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
        cout << "Paid " << amount << " using credit card" << endl;
    }
};

class PayPalPayment : public PaymentStrategy
{
public:
    // trivial override
    void pay(double amount) const override
    {
        cout << "Paid " << amount << " using PayPal" << endl;
    }
};

class ShoppingCart
{
public:
    // trivial setter
    void setPaymentStrategy(unique_ptr<PaymentStrategy> strategy)
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
    unique_ptr<PaymentStrategy> paymentStrategy;
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
    cart.setPaymentStrategy(make_unique<CreditCardPayment>());
    cart.checkout(49.99);

    cart.setPaymentStrategy(make_unique<PayPalPayment>());
    cart.checkout(19.99);
    return 0;
}
