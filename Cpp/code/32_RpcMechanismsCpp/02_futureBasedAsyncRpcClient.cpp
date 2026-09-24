#include <chrono>
#include <future>
#include <iostream>
#include <thread>

#define SIMULATED_NETWORK_DELAY_MS 200
#define OPERAND_A 10
#define OPERAND_B 32

using namespace std;

/*****************************************************************************
 * Name: simulatedRemoteAdd
 *
 * Description:
 *         Stands in for the network round trip an RPC client library makes
 *         under the hood: sleeps to simulate latency, then returns the
 *         "server's" result. See 24_Concurrency/05_futureAsyncPromise.cpp
 *         for std::async/std::future basics -- this reuses that mechanism,
 *         the only new idea here is applying it to a "remote call".
 *
 * Inputs:
 *         a : first operand.
 *         b : second operand.
 *
 * Returns:
 *         The sum of a and b, after a simulated network delay.
 *****************************************************************************/
int simulatedRemoteAdd(int a, int b)
{
    this_thread::sleep_for(chrono::milliseconds(SIMULATED_NETWORK_DELAY_MS));
    return a + b;
}

/*****************************************************************************
 * Name: addAsync
 *
 * Description:
 *         The client-side stub for a hypothetical "add" RPC. Real RPC client
 *         libraries (gRPC C++'s CompletionQueue-based async API, Thrift's
 *         async clients) expose calls this way rather than blocking, so the
 *         calling thread can keep doing other work while the network round
 *         trip is in flight.
 *
 * Inputs:
 *         a : first operand.
 *         b : second operand.
 *
 * Returns:
 *         A future the caller can wait on or poll for the remote result.
 *****************************************************************************/
future<int> addAsync(int a, int b)
{
    return async(launch::async, simulatedRemoteAdd, a, b);
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Fires off a simulated RPC call, does unrelated local work while it
 *         is in flight, then polls the future non-blockingly before finally
 *         retrieving the result -- demonstrating why real RPC clients avoid
 *         blocking the calling thread on network I/O.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    future<int> pendingResult = addAsync(OPERAND_A, OPERAND_B);

    cout << "call dispatched, caller is free to do other work...\n";
    for (int i = 0; i < 3; ++i)
    {
        future_status status = pendingResult.wait_for(chrono::milliseconds(50));
        if (status == future_status::ready)
        {
            cout << "result arrived early during poll loop\n";
            break;
        }
        cout << "still waiting on the remote call, doing local work instead\n";
    }

    int result = pendingResult.get();
    cout << "remote add(10, 32) = " << result << "\n";

    return 0;
}
