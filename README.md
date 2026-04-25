# MultiThreading-C-
Hands on code of MultiThreading

## Educational Examples

This repository contains educational examples demonstrating various core concepts of Concurrency and Multithreading in C++. These examples are designed to be simple, highly readable, and thoroughly commented to help beginners grasp how threads, mutexes, and condition variables operate together effectively.

## 📂 Files & Concepts Covered

### 1. `Condition_Variable.cpp`
A straightforward demonstration of how `std::condition_variable` can be used to synchronize threads in a simple bank account sequence.
- **Concept:** One thread (`withdraw`) waits, while another thread (`add`) updates shared states and notifies the waiting thread.
- **Key Features:** `std::mutex`, `std::unique_lock`, `std::lock_guard`, `std::condition_variable`.

### 2. `Producer_Consumer.cpp`
An implementation of the classic Producer-Consumer problem using modern C++ synchronization primitives.
- **Concept:** A producer thread creates items and pushes them into a shared buffer, while a consumer thread patiently waits to pop those items off and process them.
- **Key Features:** `std::queue` (as shared memory), wait states checking for queue fullness or emptiness, preventing race conditions or infinite loops.

### 3. `Thread_pool.cpp`
A fully functioning, yet beginner-friendly Thread Pool implementation in C++11.
- **Concept:** Instead of spinning up and tearing down expensive overhead threads for every small task, a fixed number of "worker" threads are launched once. They sit idly waiting for `std::function` tasks to be enqueued. Once a new task enters the protected queue, one of the worker threads will wake up and execute it.
- **Key Features:** `std::vector<std::thread>`, `std::queue<std::function<void()>>`, elegant queue locking, clean graceful shutdowns waiting for pending tasks to finish via thread joining.

## 🚀 How to Run
To compile any of these C++ files, you need a compiler that supports at least C++11 (like `g++` or `clang++`). 

For example, to compile and run the thread pool:
```bash
# Compile using g++
g++ -std=c++11 -pthread Thread_pool.cpp -o Thread_pool

# Execute the compiled binary
./Thread_pool
```
*(Make sure to link the pthread library by including the `-pthread` flag)*
