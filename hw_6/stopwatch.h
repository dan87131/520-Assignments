#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>

class Stopwatch {
public:
    Stopwatch();

    void start();
    void stop();
    void reset();

    double get_minutes() const;
    double get_seconds() const;
    double get_milliseconds() const;
    double get_nanoseconds() const;

private:
    void update_elapsed_time();

    bool m_running;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_stop_time;
    std::chrono::duration<double> m_elapsed_time;
};

#endif //STOPWATCH_H
