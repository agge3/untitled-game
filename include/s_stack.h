#ifndef S_STACK_H
#define S_STACK_H

//#define SFML_STATIC

#include "state.h"
#include "s_ids.h"
#include "r_ids.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <utility>
#include <functional>
#include <map>

namespace sf {
    class Event;
    class RenderWindow;
}

class StateStack : private sf::NonCopyable {
public:
    enum Action {
        Push,
        Pop,
        Clear,
    };

    explicit StateStack(State::Context context);

    // template class for different states to use to register
    template <typename T>
    void register_state(States::ID state_id);

    void update(sf::Time delta_time);
    void draw();
    void handle_event(const sf::Event& event);

    void push_state(States::ID state_id);
    void pop_state();
    // returns ID of prev state for states to request
    States::ID prev_state() const;
    void clear_states();

    bool is_empty() const;
private:
    struct PendingChange {
        explicit PendingChange(Action action,
                States::ID state_id = States::None);
        Action action;
        States::ID state_id;
    };

    State::Ptr create_state(States::ID state_id);
    void apply_pending_changes();

    std::vector<State::Ptr> m_stack;
    // stack index to keeping track of states by ID
    std::vector<States::ID> m_stack_index;
    std::vector<PendingChange> m_pending_list;
    State::Context m_context;
    std::map<States::ID, std::function<State::Ptr()>> m_factories;
};

// implementation must be in header for app to see - linker error
template <typename T>
void StateStack::register_state(States::ID state_id)
{
    // T is the derived state class, malloc for States:ID and return a smart ptr,
    // wrapper function to create any state object from stack
    // effect: register_state does not need to know the strongly typed object
    // and can be used for any s_ object (keeps dep low)
    m_factories[state_id] = [this] () {
        // for each ID return ptr to state & create state
        return State::Ptr(new T(*this, m_context));
    };
}

#endif
