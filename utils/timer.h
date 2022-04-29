#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>

using std::cout;

class Timer 
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
public:

    Timer()
    {
        _start = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        auto _finish = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(_start).time_since_epoch().count();
        auto finish = std::chrono::time_point_cast<std::chrono::microseconds>(_finish).time_since_epoch().count();

        cout << std::dec << "\n" << finish -start << " us.\n";
    }
};

#endif /* TIMER_H */
