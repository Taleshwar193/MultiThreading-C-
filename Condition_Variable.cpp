// Online C++ compiler to run C++ program online
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
using namespace std;

mutex mtx;
condition_variable cv;

int balance = 0;

void withdraw(int money) {
  unique_lock<mutex> ul(mtx);
  cv.wait(ul, [] { return (balance != 0) ? true : false; });

  if (balance >= money) {
    cout << "withdraw amount is money: " << money << endl;
    cout << "remaining amount is: " << balance << endl;
  }
}

void add(int money) {
  lock_guard<mutex> lg(mtx);

  balance += money;
  cout << "Added amount is: " << money << " and final balance is:" << balance
       << endl;

  cv.notify_one();
}

int main() {

  thread t1(withdraw, 500);
  this_thread::sleep_for(chrono::milliseconds(100));
  thread t2(add, 500);

  t1.join();
  t2.join();

  return 0;
}