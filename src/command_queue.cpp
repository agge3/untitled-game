#include "command_queue.h"
#include "scene_node.h"

void CommandQueue::push(const Command& command)
{
    m_queue.push(command);
}

Command CommandQueue::pop()
{
    Command command = m_queue.front();
    m_queue.pop();
    return command;
}

// check is command queue is empty - t/f
bool CommandQueue::is_empty() const
{
    return m_queue.empty();
}
