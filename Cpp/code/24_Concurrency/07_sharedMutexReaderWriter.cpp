#include <iostream>
#include <thread>
#include <shared_mutex>
#include <mutex>
#include <vector>

#define READER_COUNT 4
#define READER_ITERATIONS 3

using namespace std;

// Contrast with 02_mutexAndLockGuard.cpp: a plain std::mutex serializes every
// access, readers included. std::shared_mutex lets many readers hold the lock
// concurrently (std::shared_lock) while a writer still gets fully exclusive
// access (std::unique_lock) — good when reads vastly outnumber writes.
shared_mutex dataMutex;
int sharedValue = 0;

/*****************************************************************************
 * Name: readValue
 *
 * Description:
 *         Takes a shared (read) lock and prints the current value, allowing
 *         other readers to run at the same time.
 *
 * Inputs:
 *         readerId : identifier used only for the printed output.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void readValue(int readerId)
{
    for (int i = 0; i < READER_ITERATIONS; i = i + 1)
    {
        shared_lock<shared_mutex> lock(dataMutex);
        cout << "reader " << readerId << " sees " << sharedValue << "\n";
    }
}

/*****************************************************************************
 * Name: writeValue
 *
 * Description:
 *         Takes an exclusive (write) lock and updates the shared value,
 *         blocking every reader and writer until it releases the lock.
 *
 * Inputs:
 *         newValue : the value to store.
 *
 * Returns:
 *         None.
 *****************************************************************************/
void writeValue(int newValue)
{
    unique_lock<shared_mutex> lock(dataMutex);
    sharedValue = newValue;
    cout << "writer set value to " << newValue << "\n";
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Launches several concurrent readers plus a writer against a value
 *         protected by std::shared_mutex, demonstrating the reader/writer
 *         lock pattern.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    vector<thread> readers;

    for (int i = 0; i < READER_COUNT; i = i + 1)
    {
        readers.emplace_back(readValue, i);
    }

    thread writer(writeValue, 42);

    for (thread &reader : readers)
    {
        reader.join();
    }
    writer.join();

    return 0;
}
