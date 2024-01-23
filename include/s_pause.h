#pragma once

//#define SFML_STATIC

#include "state.h"
#include "sprite_node.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <imgui.h>

#include <memory>

class PauseState : public State {
public:
    PauseState(StateStack& stack, Context context);
    ~PauseState();

    // same virtual fn for every state (behave the same)
    virtual void draw();
    virtual bool update(sf::Time delta_time);
    virtual bool handle_event(const sf::Event& event);

    void gui_frame();
private:
    sf::RenderWindow& m_window;
    std::unique_ptr<SpriteNode> m_background_sprite;
    sf::Text m_paused_text;
    sf::Text m_instruction_text;
};
