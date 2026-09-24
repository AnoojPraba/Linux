#include <iostream>

using namespace std;

// Singleton: a logger that only ever has one instance.
class Logger
{
public:
    // Returns the single shared instance.
    static Logger& instance()
    {
        static Logger single;
        return single;
    }

    /*****************************************************************************
     * Name: log
     *
     * Description:
     *         Prints a message prefixed with the logger tag.
     *
     * Inputs:
     *         message : the text to log.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void log(const string& message)
    {
        cout << "[LOG] " << message << endl;
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    Logger() = default; // trivial private constructor
};

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates that Logger::instance() always yields the same object.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    Logger::instance().log("Application started");
    Logger& first = Logger::instance();
    Logger& second = Logger::instance();
    if (&first == &second)
    {
        cout << "Both references point to the same Logger instance." << endl;
    }
    return 0;
}
