#include "s_stack.h"

#include <cassert>

StateStack::StateStack(State::Context context) :
    m_stack(),
    m_pending_list(),
    m_context(context),
    m_factories()
{}

// takes an ID of a State and returns smart pointer to state
State::Ptr StateStack::create_state(States::ID state_id)
{
    auto found = m_factories.find(state_id);
    // assert a state exists
    assert(found != m_factories.end());

    // invokes the std::function factory, mallocing state and returning smart
    // ptr of state, storing it in second, mapped to ID
    return found->second();
}

// every polled event is fed into the StateStack, bc the StateStack houses all
// States, will deliver the event to the active States
void StateStack::handle_event(const sf::Event& event)
{
    // reverse it from end to begin, from highest state to lowest. if any states
    // return false exit and give control to state that doesn't want to give
    // input to any other state than itself
    for (auto it = m_stack.rbegin(); it != m_stack.rend(); ++it)
        if (!(*it)->handle_event(event))
            break;
    apply_pending_changes();
}

void StateStack::update(sf::Time delta_time)
{
    // reverse it from end to begin, stop if state.update returns false
    for (auto it = m_stack.rbegin(); it != m_stack.rend(); ++it)
        if (!(*it)->update(delta_time))
            break;
    apply_pending_changes();
}

void StateStack::draw()
{
    // draw all active states from bottom to top (top on top)
    for (State::Ptr& state : m_stack) {
        // first, clear state by drawing black rect (implemented in clear())
        //state->clear();
        // then safe to draw new state
        state->draw();
    }
}

void StateStack::push_state(States::ID state_id)
{
    m_pending_list.push_back(PendingChange(Push, state_id));
}

void StateStack::pop_state()
{
    m_pending_list.push_back(PendingChange(Pop));
}

States::ID StateStack::prev_state() const
{
    if (m_stack_index.size() < 2)
        return States::None;
    else
        return m_stack_index.at(m_stack_index.size() - 2);
}

void StateStack::clear_states()
{
    m_pending_list.push_back(PendingChange(Clear));
}

bool StateStack::is_empty() const
{
    return m_stack.empty();
}

StateStack::PendingChange::PendingChange(Action action, States::ID state_id) :
    action(action),
    state_id(state_id)
{}

// delayed processing, doesn't immediately pop/push/clear. Stores actions in
// PendingAction list first so that they can be handled safely
void StateStack::apply_pending_changes()
{
    // it through PendingChange list before manip stack to safely not lose
    // actions
    for (PendingChange change : m_pending_list) {
        switch (change.action) {
        // push calls malloc of a state and puts it on the stack
        case Push:
            m_stack.push_back(create_state(change.state_id));
            m_stack_index.push_back(change.state_id);
            break;
        case Pop:
            m_stack.pop_back();
            m_stack_index.pop_back();
            break;
        case Clear:
            m_stack.clear();
            m_stack_index.clear();
            break;
        }
    }
    // after list it complete, can clear, safely it through changes
    m_pending_list.clear();
}
