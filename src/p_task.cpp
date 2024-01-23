#include "p_task.h"

void ParallelTask::execute()
{
    m_finished = false;
    m_elapsed_time.restart();
    m_thread.launch();
}

bool ParallelTask::is_finished()
{
    sf::Lock lock(m_mutex);
    return m_finished;
}

float ParallelTask::get_completion()
{
    sf::Lock lock(m_mutex);
    // 100% at 10 sec of elapsed time, give tasks 10 sec to complete...
    return m_elapsed_time.getElapsedTime().asSeconds() / 10.f;
}

void ParallelTask::run_task()
{
    bool ended = false;
    while (!ended) {
        sf::Lock lock(m_mutex); // protect the clock
        // give tasks 10 sec to complete...
        if (m_elapsed_time.getElapsedTime().asSeconds() >= 10.f)
            ended = true;
    }

    // m_finished may be accessed from multiple threads -> protect it
    {
        sf::Lock lock(m_mutex);
        m_finished = true;
    }
}
