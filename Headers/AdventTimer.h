#ifndef _ADVENT_TIMER_H_
#define _ADVENT_TIMER_H_

#include <chrono>

struct AdventTimer
{
    using Timing = std::chrono::high_resolution_clock;
    using Point = std::chrono::time_point<Timing>;

    Point start;

    AdventTimer()
        : start{ Timing::now() }
    {}

    double current()
    {
        Point end{ Timing::now() };
        return std::chrono::duration<double, std::milli>{end - start}.count();
    }
};

#endif // _ADVENT_TIMER_H_