#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

std::mutex mtx;                     // Mutex to protect the buffer
std::condition_variable cv;         // Condition variable for synchronization
std::queue<int> buffer;             // The shared buffer queue
const unsigned int maxBufferSize = 5; // Limiting the buffer size

// Producer function
void producer(int val) {
    while (val > 0) {
        std::unique_lock<std::mutex> lock(mtx);
        
        // Wait until there's space in the buffer
        cv.wait(lock, [] { return buffer.size() < maxBufferSize; });
        
        // Produce an item
        buffer.push(val);
        std::cout << "Produced: " << val << std::endl;
        val--;
        
        lock.unlock();
        cv.notify_all(); // Notify consumer(s) that items are available
        
        // Simulate work
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    }
}

// Consumer function
void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        
        // Wait until there are items in the buffer
        cv.wait(lock, [] { return !buffer.empty(); });
        
        // Consume an item
        int val = buffer.front();
        buffer.pop();
        std::cout << "Consumed: " << val << std::endl;
        
        lock.unlock();
        cv.notify_all(); // Notify producer(s) that space is available
        
        // Exit loop when the last item (1) is consumed
        if (val == 1) break; 
        
        // Simulate work
        std::this_thread::sleep_for(std::chrono::milliseconds(150)); 
    }
}

int main() {
    std::cout << "Starting Producer-Consumer...\n";
    // Producer starts with creating numbers from 10 down to 1
    std::thread t1(producer, 10);
    std::thread t2(consumer);
    
    t1.join();
    t2.join();
    
    std::cout << "Finished Producer-Consumer.\n";
    return 0;
}
