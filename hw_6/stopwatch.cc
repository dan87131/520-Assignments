#include "stopwatch.h"

Stopwatch::Stopwatch() : m_running(false), m_elapsed_time(std::chrono::duration<double>::zero()) {}

void Stopwatch::start() {
    if (!m_running) {
        m_running = true;
        m_start_time = std::chrono::high_resolution_clock::now();
    }
}

void Stopwatch::stop() {
    if (m_running) {
        m_running = false;
        m_stop_time = std::chrono::high_resolution_clock::now();
        update_elapsed_time();
    }
}

void Stopwatch::reset() {
    m_running = false;
    m_elapsed_time = std::chrono::duration<double>::zero();
}

double Stopwatch::get_minutes() const {
    return std::chrono::duration_cast<std::chrono::minutes>(m_elapsed_time).count();
}

double Stopwatch::get_seconds() const {
    return m_elapsed_time.count();
}

double Stopwatch::get_milliseconds() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(m_elapsed_time).count();
}

double Stopwatch::get_nanoseconds() const {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(m_elapsed_time).count();
}

void Stopwatch::update_elapsed_time() {
    if (m_running) {
        m_elapsed_time = std::chrono::high_resolution_clock::now() - m_start_time;
    } else {
        m_elapsed_time = m_stop_time - m_start_time;
    }
}
