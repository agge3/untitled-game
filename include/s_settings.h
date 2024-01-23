#pragma once

#include "state.h"
#include "player.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <imgui.h>

class SettingsState : public State {
public:

    SettingsState(StateStack& stack, Context context);
    ~SettingsState();

    virtual void draw();
    virtual bool update(sf::Time delta_time);
    virtual bool handle_event(const sf::Event& event);

    void gui_frame();
    void change_keybindings();
private:
    enum class GuiMenu : unsigned int {
        Main = 0,
        Keybindings = 1 << 0,
        Other = 1 << 1,
    };

    sf::RenderWindow& m_window;
    sf::Sprite m_background_sprite;
    sf::Vector2f m_gui_size;
    sf::Vector2f m_gui_pos;
    GuiMenu m_gui_menu;
    // create local action enum to switch through and assign new player actions
    Player::Action m_action;
    bool m_assigning_keys;

    // imgui flags & no close button for all guis of state
    const static ImGuiWindowFlags m_imflags = ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoSavedSettings;
    bool m_no_imclose = true;
};
