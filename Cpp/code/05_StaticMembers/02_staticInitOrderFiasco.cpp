#include <iostream>
#include <string>

using namespace std;

// The "static initialization order fiasco": C++ does NOT guarantee the order
// in which non-local static objects with static storage duration are
// constructed across DIFFERENT translation units (TUs). If a static object in
// one TU has a constructor that depends on a static object defined in
// another TU also having already been constructed, the program's behavior
// depends on link order / TU processing order, which the standard leaves
// unspecified.
//
// A true demonstration needs two separate .cpp files linked together (this
// repo's Makefile builds each .cpp independently into its own binary --
// see Cpp/code/Makefile's generic "$(BIN_DIR)/%: %.cpp" rule -- so it has no
// easy way to link two source files into one binary). This file is therefore
// a single-file illustrative simplification: it models what a second TU's
// static object WOULD look like, with comments marking the conceptual split,
// so the ordering hazard and its fix can still be shown and compiled.

#define FIASCO_LOG_PREFIX "[FiascoDemo] "

// --- Conceptually "translation unit A" ---
class Logger
{
    private:
        string prefix;

    public:
        // Trivial constructor with initializer list.
        Logger(const string &logPrefix) : prefix(logPrefix) {}

        /*****************************************************************************
         * Name: log
         *
         * Description:
         *         Prints a message prefixed with this Logger's prefix.
         *
         * Inputs:
         *         message : the text to log.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        void log(const string &message) const
        {
            cout << prefix << message << "\n";
        }
};

// If this were namespace-scope in TU A, e.g. "Logger globalLogger(...)", and
// another TU B had its own namespace-scope static object whose CONSTRUCTOR
// called globalLogger.log(...), there would be no guarantee globalLogger has
// already been constructed by the time B's static object runs -- if B's
// static happened to be constructed first (order across TUs is unspecified),
// globalLogger would still be uninitialized memory, and calling log() on it
// would be undefined behavior (commonly a crash or garbage output in
// practice).

/*****************************************************************************
 * Name: getLogger
 *
 * Description:
 *         "Construct on first use" idiom: wraps the static Logger in a
 *         function returning a reference to a function-local static. Since
 *         C++11, initialization of a function-local static is guaranteed to
 *         happen the first time control passes through its declaration, and
 *         is guaranteed thread-safe ("magic statics") -- no other TU can
 *         observe it in a partially-constructed state, and there is no
 *         cross-TU ordering problem because it is constructed lazily, on
 *         first call, rather than at some unspecified point during program
 *         startup.
 *
 * Returns:
 *         Reference to the single, lazily-constructed Logger instance.
 *****************************************************************************/
Logger &getLogger()
{
    static Logger instance(FIASCO_LOG_PREFIX);
    return instance;
}

// --- Conceptually "translation unit B" ---
class StartupNotifier
{
    public:
        /*****************************************************************************
         * Name: StartupNotifier
         *
         * Description:
         *         Depends on the Logger being fully constructed. Using
         *         getLogger() instead of a plain global guarantees that,
         *         regardless of what TU this constructor runs from or when.
         *
         * Returns:
         *         None.
         *****************************************************************************/
        StartupNotifier()
        {
            getLogger().log("StartupNotifier constructed safely");
        }
};

// If StartupNotifier were itself a namespace-scope static in TU B, its
// constructor calling getLogger() (rather than a plain global Logger) is
// what makes this safe regardless of TU construction order.
static StartupNotifier startupNotifier;

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates that getLogger() is safely usable from anywhere,
 *         including before main() runs (via the static StartupNotifier).
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    getLogger().log("main() running");
    return 0;
}
