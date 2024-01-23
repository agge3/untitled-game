#pragma once

//#define SFML_STATIC

#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Clock.hpp>

class ParallelTask {
public:
    // default &run task & &p_task
    ParallelTask() : m_thread(&ParallelTask::run_task, this),
        m_finished(false) {}

    void execute();
    bool is_finished();
    float get_completion();
private:
    void run_task();

    sf::Thread m_thread;
    bool m_finished;
    sf::Clock m_elapsed_time;
    sf::Mutex m_mutex;
};
