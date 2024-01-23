#include "s_menu.h"
#include "utility.h"
#include "r_holders.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <iostream>

MenuState::MenuState(StateStack& stack, Context context) :
    State(stack, context),
    m_options(),
    m_options_index(0)
{
    // get title screen from context, already in mem, don't need to recreate
    m_background_sprite.setTexture(context.textures->get(Textures::TitleScreen));
    scale_sprite(m_background_sprite);
    // get main font from context, already in mem, don't need to recreate
    sf::Font& font = context.fonts->get(Fonts::Main);

    // MENU:
    // play option...
    sf::Text play_option;
    play_option.setFont(font);
    play_option.setString("Play");
    center_origin(play_option);
    // (x, y) px of play
    play_option.setPosition(640, 600);
    // add to menu options vector
    m_options.push_back(play_option);

    // settings option...
    sf::Text settings_option;
    settings_option.setFont(font);
    settings_option.setString("Settings");
    center_origin(settings_option);
    // (x, y) px of exit, -30 px y offset from play
    settings_option.setPosition(640, 630);
    // add to menu options vector
    m_options.push_back(settings_option);

    // exit option...
    sf::Text exit_option;
    exit_option.setFont(font);
    exit_option.setString("Exit");
    center_origin(exit_option);
    // (x, y) px of exit, -30 px y offset from settings
    exit_option.setPosition(640, 660);
    // add to menu options vector
    m_options.push_back(exit_option);

    update_option_text();
}

void MenuState::draw()
{
    // get context of window, already in mem, don't recreate
    sf::RenderWindow& window = *get_context().window;

    // draw menu screen and set view
    window.setView(window.getDefaultView());
    window.draw(m_background_sprite);
    // for each menu option draw
    for (const sf::Text& text : m_options)
        window.draw(text);
}

/**
 * Update cycle for menu state.
 * @param sf::Time delta_time (game delta time).
 * @return Returns true to keep previous state updating.
 * @remark Delta time is a parameter for each state's update() - unused if not
 * needed! Like in a menu...!
 */
bool MenuState::update(sf::Time delta_time)
{
    return true;
}

/**
 * Handle events for menu state.
 * @param sf::Event& event (to be handled).
 * @return Returns false to keep previous state from handling events.
 */
bool MenuState::handle_event(const sf::Event& event)
{
    // logic for each option...
    if (event.type != sf::Event::KeyPressed)
        return false; // don't handle event if no key is pressed!
    if (event.key.code == sf::Keyboard::Return) {
        if(m_options_index == Play) {
            // if play is pressed ->
            // pop off menu state & push game state to top of stack
            request_pop_stack();
            request_push_stack(States::Game);
        } else if (m_options_index == Settings) {
            request_push_stack(States::Settings);
        } else if (m_options_index == Exit) {
            // if exit is pressed ->
            // pop off menu state -> stack will be empty and game will close...
            request_pop_stack();
        }
    } else if (event.key.code == sf::Keyboard::Up) {
        // decrement option and wrap-around
        if (m_options_index > 0)
            --m_options_index;
        else
            m_options_index = m_options.size() - 1;
        update_option_text();
    } else if (event.key.code == sf::Keyboard::Down) {
        // increment option and wrap-around
        if (m_options_index < m_options.size() - 1)
            ++m_options_index;
        else
            m_options_index = 0;
        update_option_text();
    }

    return false;
}

void MenuState::update_option_text()
{
    // guard action in case options empty...
    if (!m_options.empty()) {
        // white all option text
        for (sf::Text& text : m_options)
            text.setFillColor(sf::Color::White);
        // red selected option text
        m_options[m_options_index].setFillColor(sf::Color::Red);
    }
}

MenuState::~MenuState()
{
    std::cout << "Menu state has been destroyed!" << std::endl;
}
