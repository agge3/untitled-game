#include "s_title.h"
#include "r_holders.h"
#include "utility.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>

TitleState::TitleState(StateStack& stack, Context context) :
    State(stack, context),
    m_text(),
    m_show_text(true),
    m_text_effect_time(sf::Time::Zero)
{
    // get background texture from context, scale sprite to resolution
    m_background_sprite.setTexture(context.textures->get(Textures::TitleScreen));
    scale_sprite(m_background_sprite);
    // get main font from context
    m_text.setFont(context.fonts->get(Fonts::Main));

    m_text.setString("Press any key to start");
    center_origin(m_text);

    // text position (x, y) px
    m_text.setPosition(640, 600);
}

void TitleState::draw()
{
    // &window = &context.window
    sf::RenderWindow& window = *get_context().window;
    // make sure view is set to initial resolution
    window.setView(window.getDefaultView());
    window.draw(m_background_sprite);

    // bool t/f to flicker text
    if (m_show_text)
        window.draw(m_text);
}

bool TitleState::update(sf::Time delta_time)
{
    m_text_effect_time += delta_time;
    // flicker text every 0.5 sec
    if (m_text_effect_time >= sf::seconds(0.5f)) {
        m_show_text = !m_show_text;
        m_text_effect_time = sf::Time::Zero; // reset timer to 0
    }

    return true;
}

bool TitleState::handle_event(const sf::Event& event)
{
    // if any key is pressed, push the next state (menu)
    if (event.type == sf::Event::KeyPressed) {
        request_pop_stack();
        request_push_stack(States::Menu);
    }

    return true;
}

TitleState::~TitleState()
{
    std::cout << "Title state has been destroyed!" << std::endl;
}
