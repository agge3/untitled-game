//#pragma once
#ifndef STACK_H
#define STACK_H

//#define SFML_STATIC

#include "s_ids.h"
#include "r_ids.h"
#include "r_holders.h"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>

class StateStack;
class Player;

class State {
public:
    typedef std::unique_ptr<State> Ptr;
    // Context keeps shared obj between states, "context" to avoid wasting mem
    // and having to load texture, etc. in mult mem locations
    // saves sys mem by reusing same textures, etc. for every state & provides
    // access to window's view at all times, for pos, resize, objects relative
    // to views dimensions
    struct Context {
        Context(sf::RenderWindow& window, TextureHolder& textures,
                FontHolder& fonts, Player& player);
        sf::RenderWindow *window;
        TextureHolder *textures;
        FontHolder *fonts;
        Player *player;
    };

    State(StateStack& stack, Context context);
    virtual ~State();

    // virtual fn for every state to inherit (and use)
    virtual void draw() = 0;
    virtual bool update(sf::Time delta_time) = 0;
    virtual bool handle_event(const sf::Event& event) = 0;

    // non-virtual clear fn for each state to use to clear screen (if desired)
    //void clear();
protected:
    void request_push_stack(States::ID state_id);
    void request_pop_stack();
    // returns ID of prev state
    States::ID request_prev_state() const;
    void request_clear_state();

    Context get_context() const;
private:
    StateStack *m_stack;
    Context m_context;
};

#endif
