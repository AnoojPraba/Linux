#include <iostream>
#include <memory>

using namespace std;

// VIOLATION: the high-level class directly constructs and depends on a
// concrete low-level class, coupling policy to a specific implementation.
class MySqlDatabase
{
public:
    // trivial method
    void save(const string &data) const
    {
        cout << "Saving to MySQL: " << data << endl;
    }
};

class UserServiceBad
{
public:
    /*****************************************************************************
     * Name: registerUser
     *
     * Description:
     *         Saves a new user, directly coupled to MySqlDatabase with no
     *         way to substitute a different storage backend.
     *
     * Inputs:
     *         name : the user name to register.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void registerUser(const string &name) const
    {
        database.save(name);
    }

private:
    MySqlDatabase database;
};

// FIX: both the high-level and low-level modules depend on an abstraction,
// which is injected in via the constructor (constructor injection).
class Database
{
public:
    virtual ~Database() = default;
    virtual void save(const string &data) const = 0;
};

class MySqlDatabaseGood : public Database
{
public:
    // trivial override
    void save(const string &data) const override
    {
        cout << "Saving to MySQL: " << data << endl;
    }
};

class InMemoryDatabase : public Database
{
public:
    // trivial override
    void save(const string &data) const override
    {
        cout << "Saving to in-memory store: " << data << endl;
    }
};

class UserService
{
public:
    // constructor injection of the abstraction
    explicit UserService(shared_ptr<Database> db) : database(std::move(db)) {}

    /*****************************************************************************
     * Name: registerUser
     *
     * Description:
     *         Saves a new user through whichever Database implementation was
     *         injected, with no compile-time dependency on a concrete type.
     *
     * Inputs:
     *         name : the user name to register.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void registerUser(const string &name) const
    {
        database->save(name);
    }

private:
    shared_ptr<Database> database;
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates the DIP violation followed by the fixed design where
 *         UserService depends only on the Database abstraction and the
 *         concrete implementation is injected.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    UserServiceBad badService;
    badService.registerUser("alice");

    UserService mysqlService(make_shared<MySqlDatabaseGood>());
    mysqlService.registerUser("bob");

    UserService testService(make_shared<InMemoryDatabase>());
    testService.registerUser("carol");

    return 0;
}
