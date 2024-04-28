#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <thread>

// Number of increments each thread performs
const int NUM_INCREMENTS = 500;

namespace ThreadSafeIncrement {
    // Mutex to synchronize access to shared variable
    std::mutex sharedVariableMutex;

    // Condition variable to synchronize thread execution
    std::condition_variable conditionVariable;

    // Flag indicating whose turn is it
    bool isThread1Turn = false;

    // Shared variable to be incremented
    long long sharedVariable = 0;

    // Function for threads to safely increment the shared variable
    void safeIncrement(bool isThread1) {
        for (int i = 0; i < NUM_INCREMENTS; ++i) {
            // Lock the mutex
            std::unique_lock<std::mutex> lock(sharedVariableMutex);

            // Wait until it's this thread's turn
            conditionVariable.wait(lock, [isThread1]{ return isThread1 == isThread1Turn; });

            // Increment the shared variable
            ++sharedVariable;

            // Change the turn
            isThread1Turn = !isThread1Turn;

            // Notify the other thread
            conditionVariable.notify_one();
        }
    }
}

int main() {
    using namespace ThreadSafeIncrement;

    // Thread 1 first
    std::thread thread1(safeIncrement, false);

    // Thread 2 next
    std::thread thread2(safeIncrement, true);

    // Join the threads
    thread1.join();
    thread2.join();

    // Print the value of the shared variable after both threads have finished
    std::cout << "Shared Variable: " << sharedVariable << std::endl;

    return 0;
}

