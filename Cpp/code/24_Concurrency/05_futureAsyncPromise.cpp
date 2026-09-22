#include <iostream>
#include <future>
#include <thread>

#define OPERAND_A 6
#define OPERAND_B 7

/*****************************************************************************
 * Name: multiply
 *
 * Description:
 *         Multiplies two integers, used as the callable passed to
 *         std::async so its result can be retrieved via a std::future.
 *
 * Inputs:
 *         a : first operand.
 *         b : second operand.
 *
 * Returns:
 *         The product of a and b.
 *****************************************************************************/
int multiply(int a, int b)
{
    return a * b;
}

/*****************************************************************************
 * Name: produceViaPromise
 *
 * Description:
 *         Computes a value on a background thread and delivers it through a
 *         std::promise, mirroring what std::async does automatically.
 *
 * Inputs:
 *         resultPromise : the promise the computed value is set on.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void produceViaPromise(std::promise<int> resultPromise)
{
    resultPromise.set_value(multiply(OPERAND_A, OPERAND_B));
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Demonstrates std::async/std::future for a fire-and-forget
 *         computation, then demonstrates the lower-level
 *         std::promise/std::future pair for the same computation.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    std::future<int> asyncResult = std::async(std::launch::async, multiply, OPERAND_A, OPERAND_B);
    std::cout << "async result = " << asyncResult.get() << "\n";

    std::promise<int> promise;
    std::future<int> promiseResult = promise.get_future();
    std::thread producerThread(produceViaPromise, std::move(promise));
    std::cout << "promise result = " << promiseResult.get() << "\n";
    producerThread.join();

    return 0;
}
