#include "state.h"
#include "s_stack.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures,
        FontHolder& fonts, Player& player) :
    window(&window),
    textures(&textures),
    fonts(&fonts),
    player(&player) {}

State::State(StateStack& stack, Context context) :
    m_stack(&stack),
    m_context(context) {}

State::~State() {}

/*void State::clear() {
    // get window from context & set view to full window
    Context.window->setView(window->getDefaultView());
    // create rect (size of window view) & set color to black
    sf::RectangleShape rect(Context::window->getDefaultView().getSize());
    rect.setFillColor(sf::Color::Black);
    // draw black 1 frame to clear screen
    Context::window->draw(rect);
}*/

void State::request_push_stack(States::ID state_id)
{
    m_stack->push_state(state_id);
}

void State::request_pop_stack()
{
    m_stack->pop_state();
}

States::ID State::request_prev_state() const
{
    return m_stack->prev_state();
}

void State::request_clear_state()
{
    m_stack->clear_states();
}

State::Context State::get_context() const
{
    return m_context;
}
