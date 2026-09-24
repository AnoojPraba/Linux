#include <iostream>
#include <memory>
#include <string>

#define L1_LIMIT 100.0
#define L2_LIMIT 1000.0

using namespace std;

// Handler: each link decides to handle the request or pass it further down.
class ApprovalHandler
{
public:
    explicit ApprovalHandler(unique_ptr<ApprovalHandler> nextIn)
        : next(std::move(nextIn)) {}
    virtual ~ApprovalHandler() = default;

    /*****************************************************************************
     * Name: approve
     *
     * Description:
     *         Attempts to approve the given expense amount at this level; if
     *         the amount exceeds what this handler may authorize, forwards the
     *         request to the next handler in the chain.
     *
     * Inputs:
     *         amount : the expense amount requesting approval.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void approve(double amount) const
    {
        if (canApprove(amount))
        {
            cout << name() << " approved expense of " << amount << endl;
        }
        else if (next)
        {
            next->approve(amount);
        }
        else
        {
            cout << "No approver available for expense of " << amount << endl;
        }
    }

protected:
    virtual bool canApprove(double amount) const = 0;
    virtual string name() const = 0;

private:
    unique_ptr<ApprovalHandler> next;
};

class TeamLead : public ApprovalHandler
{
public:
    // ctor forwards the next handler in the chain
    explicit TeamLead(unique_ptr<ApprovalHandler> next)
        : ApprovalHandler(std::move(next)) {}

protected:
    // trivial predicate
    bool canApprove(double amount) const override
    {
        return amount <= L1_LIMIT;
    }

    // trivial identifier
    string name() const override
    {
        return "TeamLead";
    }
};

class Director : public ApprovalHandler
{
public:
    // ctor forwards the next handler in the chain
    explicit Director(unique_ptr<ApprovalHandler> next)
        : ApprovalHandler(std::move(next)) {}

protected:
    // trivial predicate
    bool canApprove(double amount) const override
    {
        return amount <= L2_LIMIT;
    }

    // trivial identifier
    string name() const override
    {
        return "Director";
    }
};

class VicePresident : public ApprovalHandler
{
public:
    // ctor: end of chain, no further handler
    VicePresident() : ApprovalHandler(nullptr) {}

protected:
    // a VP can approve any amount, so this stage always accepts
    bool canApprove(double amount) const override
    {
        (void)amount;
        return true;
    }

    // trivial identifier
    string name() const override
    {
        return "VicePresident";
    }
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Builds an expense-approval chain (TeamLead -> Director ->
 *         VicePresident) and routes several amounts through it to show each
 *         handler either approving or escalating.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    unique_ptr<ApprovalHandler> chain =
        make_unique<TeamLead>(make_unique<Director>(make_unique<VicePresident>()));

    chain->approve(50.0);
    chain->approve(500.0);
    chain->approve(5000.0);
    return 0;
}
