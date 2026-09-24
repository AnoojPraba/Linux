#include <any>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#define OPERAND_A 3
#define OPERAND_B 4

// Simulates a "remote" call: type-erased arguments in, type-erased result out.
// A real client/server would marshal these to/from bytes on the wire; here the
// dispatch table itself is the part worth studying, so no sockets are involved.

using namespace std;

using ArgList = vector<any>;
using RpcHandler = function<any(const ArgList &)>;

// Thrown when a client asks the registry for a procedure name it doesn't know.
class UnknownProcedureError : public runtime_error
{
public:
    // trivial ctor
    explicit UnknownProcedureError(const string &name)
        : runtime_error("unknown procedure: " + name) {}
};

// Thrown when the type-erased arguments don't match what the handler expects.
class ArgumentTypeError : public runtime_error
{
public:
    // trivial ctor
    explicit ArgumentTypeError(const string &name)
        : runtime_error("bad argument types for procedure: " + name) {}
};

/*****************************************************************************
 * Name: DispatchTable
 *
 * Description:
 *         A name -> handler registry standing in for the server side of an
 *         RPC framework. Real frameworks (gRPC, Thrift) generate an
 *         equivalent table from a schema file; here it is built by hand to
 *         show what that generated code is doing underneath.
 *****************************************************************************/
class DispatchTable
{
public:
    /*****************************************************************************
     * Name: registerProcedure
     *
     * Description:
     *         Registers a callable under a procedure name so it can later be
     *         invoked by name via dispatch().
     *
     * Inputs:
     *         name    : the procedure name clients will call by.
     *         handler : the type-erased handler to invoke for that name.
     *
     * Returns:
     *         None.
     *****************************************************************************/
    void registerProcedure(const string &name, RpcHandler handler)
    {
        handlers[name] = std::move(handler);
    }

    /*****************************************************************************
     * Name: dispatch
     *
     * Description:
     *         Looks up a procedure by name and invokes it with the supplied
     *         type-erased arguments, mirroring how a server-side stub decodes
     *         a wire request and calls the matching local function.
     *
     * Inputs:
     *         name : the procedure name to invoke.
     *         args : the type-erased arguments to pass to the handler.
     *
     * Returns:
     *         The type-erased result produced by the handler.
     *****************************************************************************/
    any dispatch(const string &name, const ArgList &args) const
    {
        auto it = handlers.find(name);
        if (it == handlers.end())
        {
            throw UnknownProcedureError(name);
        }

        try
        {
            return it->second(args);
        }
        catch (const bad_any_cast &)
        {
            throw ArgumentTypeError(name);
        }
    }

private:
    unordered_map<string, RpcHandler> handlers;
};

/*****************************************************************************
 * Name: add
 *
 * Description:
 *         The "remote" procedure exposed under the name "add".
 *
 * Inputs:
 *         a : first operand.
 *         b : second operand.
 *
 * Returns:
 *         The sum of a and b.
 *****************************************************************************/
int add(int a, int b)
{
    return a + b;
}

/*****************************************************************************
 * Name: toUpper
 *
 * Description:
 *         The "remote" procedure exposed under the name "toUpper".
 *
 * Inputs:
 *         text : the string to convert.
 *
 * Returns:
 *         An upper-cased copy of text.
 *****************************************************************************/
string toUpper(const string &text)
{
    string result = text;
    for (char &c : result)
    {
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    }
    return result;
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Builds a dispatch table, registers two procedures behind
 *         type-erased handlers, then dispatches calls by name -- including
 *         the unknown-procedure and wrong-argument-type error paths.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    DispatchTable table;

    table.registerProcedure("add", [](const ArgList &args) -> any
    {
        int a = any_cast<int>(args.at(0));
        int b = any_cast<int>(args.at(1));
        return any(add(a, b));
    });

    table.registerProcedure("toUpper", [](const ArgList &args) -> any
    {
        string text = any_cast<string>(args.at(0));
        return any(toUpper(text));
    });

    any sumResult = table.dispatch("add", ArgList{OPERAND_A, OPERAND_B});
    cout << "add(3, 4) = " << any_cast<int>(sumResult) << "\n";

    any upperResult = table.dispatch("toUpper", ArgList{string("hello")});
    cout << "toUpper(\"hello\") = " << any_cast<string>(upperResult) << "\n";

    try
    {
        table.dispatch("subtract", ArgList{OPERAND_A, OPERAND_B});
    }
    catch (const UnknownProcedureError &e)
    {
        cout << "caught expected error: " << e.what() << "\n";
    }

    try
    {
        table.dispatch("add", ArgList{string("not"), string("ints")});
    }
    catch (const ArgumentTypeError &e)
    {
        cout << "caught expected error: " << e.what() << "\n";
    }

    return 0;
}
