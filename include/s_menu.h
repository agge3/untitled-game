#pragma once

//#define SFML_STATIC

#include "state.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class MenuState : public State {
public:
    MenuState(StateStack& stack, Context context);
    ~MenuState();

    virtual void draw();
    virtual bool update(sf::Time delta_time);
    virtual bool handle_event(const sf::Event& event);

    void update_option_text();
private:
    enum Option {
        Play,
        Settings,
        Exit,
    };

    sf::Sprite m_background_sprite;
    std::vector<sf::Text> m_options;
    std::size_t m_options_index;
};
