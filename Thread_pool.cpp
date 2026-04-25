#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>
#include <chrono>

using namespace std;

// Mutex to make sure cout doesn't get jumbled up from multiple threads
mutex print_mutex;

class ThreadPool {
private:
    vector<thread> workers;            // Threads that execute the tasks
    queue<function<void()>> tasks;     // Queue of tasks to be executed

    mutex queue_mutex;                      // Protects the task queue
    condition_variable condition;           // Notifies threads when there are new tasks
    bool stop;                                   // Indicates if the pool is shutting down

public:
    // Constructor launches the specified number of worker threads
    ThreadPool(size_t num_threads) : stop(false) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back([this] {
                // Each worker thread runs this infinite loop
                while (true) {
                    function<void()> task;

                    {
                        // Lock the queue before checking for tasks
                        unique_lock<mutex> lock(this->queue_mutex);

                        // Wait until there is a task OR the pool is stopped
                        this->condition.wait(lock, [this] {
                            return this->stop || !this->tasks.empty();
                        });

                        // If the pool is shutting down and there are no tasks left, exit the loop
                        if (this->stop && this->tasks.empty()) {
                            return;
                        }

                        // Grab the task from the front of the queue
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    } // Mutex is unlocked here

                    // Execute the grabbed task
                    // This happens outside the lock so other threads can still access the queue
                    task();
                }
            });
        }
    }

    // Add new work to the thread pool
    void enqueueTask(function<void()> task) {
        {
            unique_lock<mutex> lock(queue_mutex);
            if (stop) {
                throw runtime_error("Cannot enqueue on stopped ThreadPool");
            }
            tasks.push(task);
        }
        // Wake up exactly one worker thread to process the newly added task
        condition.notify_one();
    }

    // Destructor stops the pool and safely waits for all threads to finish
    ~ThreadPool() {
        {
            unique_lock<mutex> lock(queue_mutex);
            stop = true; // Tell threads it's time to stop
        }
        
        // Wake up all threads so they check the `stop` flag and exit gracefully
        condition.notify_all();
        
        // Wait for all worker threads to complete their current tasks and exit
        for (thread& worker : workers) {
            worker.join();
        }
    }
};

int main() {
    cout << "Starting Thread Pool with 3 worker threads...\n\n";
    
    // Create the Thread Pool
    ThreadPool pool(3);

    // Give 8 separate tasks to the thread pool
    for (int i = 1; i <= 8; ++i) {
        // Enqueue a lambda function as a task
        pool.enqueueTask([i] {
            // Log when a task starts
            {
                lock_guard<mutex> lock(print_mutex);
                cout << "Task " << i << " started by thread: " << this_thread::get_id() << "\n";
            }

            // Simulate some slow work taking 500 milliseconds
            this_thread::sleep_for(chrono::milliseconds(500));

            // Log when a task finishes
            {
                lock_guard<mutex> lock(print_mutex);
                cout << "Task " << i << " finished.\n";
            }
        });
    }

    cout << "All 8 tasks have been enqueued to the pool!\n\n";
    
    // We don't need to explicitly wait here.
    // Once the `pool` object goes out of scope at the end of `main`, the ThreadPool's 
    // destructor will be called. That destructor handles waiting for all tasks to finish safely.
    
    return 0;
}
