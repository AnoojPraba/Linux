#include <iostream>
#include <memory>

// VIOLATION: the high-level class directly constructs and depends on a
// concrete low-level class, coupling policy to a specific implementation.
class MySqlDatabase
{
public:
    // trivial method
    void save(const std::string &data) const
    {
        std::cout << "Saving to MySQL: " << data << std::endl;
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
    void registerUser(const std::string &name) const
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
    virtual void save(const std::string &data) const = 0;
};

class MySqlDatabaseGood : public Database
{
public:
    // trivial override
    void save(const std::string &data) const override
    {
        std::cout << "Saving to MySQL: " << data << std::endl;
    }
};

class InMemoryDatabase : public Database
{
public:
    // trivial override
    void save(const std::string &data) const override
    {
        std::cout << "Saving to in-memory store: " << data << std::endl;
    }
};

class UserService
{
public:
    // constructor injection of the abstraction
    explicit UserService(std::shared_ptr<Database> db) : database(std::move(db)) {}

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
    void registerUser(const std::string &name) const
    {
        database->save(name);
    }

private:
    std::shared_ptr<Database> database;
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

    UserService mysqlService(std::make_shared<MySqlDatabaseGood>());
    mysqlService.registerUser("bob");

    UserService testService(std::make_shared<InMemoryDatabase>());
    testService.registerUser("carol");

    return 0;
}
