#pragma once

#include "command.h"

#include <queue>

/**
 * @class CommandQueue
 * FIFO - to act as a wrapper for commands to be queued.
 */
class CommandQueue {
public:
    void push(const Command& command);
    Command pop();
    bool is_empty() const;
private:
    /// The three public methods forward commands to underlying private std::queue.
    std::queue<Command> m_queue;
};
