#pragma once

//#define SFML_STATIC

#include "state.h"
#include "s_stack.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class TitleState : public State {
public:
    TitleState(StateStack& stack, Context context);
    ~TitleState();

    virtual void draw();
    virtual bool update(sf::Time delta_time);
    virtual bool handle_event(const sf::Event& event);
private:
    sf::Sprite m_background_sprite;
    sf::Text m_text;
    bool m_show_text;
    sf::Time m_text_effect_time;
};
