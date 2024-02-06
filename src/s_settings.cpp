#include "s_settings.h"
#include "utility.h"
#include "r_holders.h"
#include "imguistyle.h"

#include <SFML/Graphics/Color.hpp>
#include <imgui-SFML.h>

#include <stdexcept>
#include <iostream>

SettingsState::SettingsState(StateStack& stack, Context context) :
    State(stack, context),
    // init state window to render window from context
    m_window(*get_context().window),
    m_background_sprite(),
    m_gui_size(),
    m_gui_pos(),
    m_gui_menu(GuiMenu::Main),
    m_action(Player::Action::None),
    m_assigning_keys(false)
{

    // set view to full size of render window
    m_window.setView(m_window.getDefaultView());
    // set different background sprite if prev state is in-game
    if (request_prev_state() == States::Game
            || request_prev_state() == States::Loading
            || request_prev_state() == States::Pause) {
        // background sprite is opacity filter over prev state
        m_background_sprite.setColor(sf::Color(0, 0, 0, 150));
    } else {
        // default background sprite
        m_background_sprite.setTexture(context.textures->get(
                    Textures::TitleScreen)); // same as title screen
        scale_sprite(m_background_sprite); // scale to curr res
        // apply opacity filter - looks better!
        //m_background_sprite.setColor(sf::Color(0, 0, 0, 150));
    }

    // init imgui sfml backend upon state creation
    if (!ImGui::SFML::Init(m_window) == -1) // -1 is return of init failure
        throw std::runtime_error("Settings GUI failed to initialize!");
    // use custom imgui style
    ImGui::SetupImGuiStyle();

    // default gui size & pos:
    // (left) gui_pos + gui_size + (right) gui_pos = 1.0, to center gui window
    m_gui_size = m_gui_pos
        = static_cast<sf::Vector2f>(m_window.getView().getSize());
    std::cout << "View size: " << m_gui_size.x << "x*" << m_gui_size.y << "y\n";

    scale_vector2f(m_gui_size, 0.6f);
    std::cout << "GUI size: " << m_gui_size.x << "x*" << m_gui_size.y << "y\n";

    scale_vector2f(m_gui_pos, 0.2f);
    std::cout << "GUI position: " << m_gui_pos.x << "x*" << m_gui_pos.y << "y"
        << std::endl;
}

void SettingsState::draw()
{
    // draw background
    m_window.draw(m_background_sprite);
    // render gui
    ImGui::SFML::Render(m_window); // frame ready -> now render in render loop
}

bool SettingsState::update(sf::Time delta_time)
{
    // gui:
    ImGui::SFML::Update(m_window, delta_time); // new frame - begin

    // switch between curr gui menu
    switch(m_gui_menu) {
    case GuiMenu::Main:
        gui_frame();
        break;
    case GuiMenu::Keybindings:
        change_keybindings();
        break;
    default:
        // default is main gui frame
        gui_frame();
        break;
    }

    ImGui::EndFrame(); // end frame - end
    return false; // deny updates to lower state(s) in stack
}

// only for processing events!
bool SettingsState::handle_event(const sf::Event& event)
{
    if (m_assigning_keys && event.type == sf::Event::KeyReleased) {
        switch (m_action) {
        case Player::Action::MoveUp: {
            std::cout << "\"Move up\" has been assigned" << std::endl;
            get_context().player->assign_key(Player::Action::MoveUp,
                    event.key.code);
            m_assigning_keys = false;
            break;
            }
        case Player::Action::MoveDown: {
            std::cout << "\"Move down\" has been assigned" << std::endl;
            get_context().player->assign_key(Player::Action::MoveDown,
                    event.key.code);
            m_assigning_keys = false;
            break;
            }
        case Player::Action::MoveLeft: {
            std::cout << "\"Move left\" has been assigned" << std::endl;
            get_context().player->assign_key(Player::Action::MoveLeft,
                    event.key.code);
            m_assigning_keys = false;
            break;
            }
        case Player::Action::MoveRight: {
            std::cout << "\"Move right\" has been assigned" << std::endl;
            get_context().player->assign_key(Player::Action::MoveRight,
                    event.key.code);
            m_assigning_keys = false;
            break;
            }
        default:
            m_assigning_keys = false;
            break;
        }
    }

    ImGui::SFML::ProcessEvent(m_window, event);

    return false; // deny handling events to lower state(s) in stack
}

void SettingsState::gui_frame()
{
    ImGui::SetNextWindowPos(m_gui_pos);
    ImGui::SetNextWindowSize(m_gui_size, ImGuiCond_FirstUseEver);

    // button x & y, spacing, & count
    ImButton buttons{{100.f, 25.f}, 5.f, 2.f};

    ImGui::Begin("Settings", &m_no_imclose, m_imflags);

    // prepare -> center buttons on menu
    sf::Vector2f centered = center_imbutton(m_gui_size, buttons);

    // button layout
    ImGui::SetCursorPos(centered); // set starting pos of button
    if (ImGui::Button("Play", buttons.vec2)) {
        request_push_stack(States::Game);
    }

    // next button y centered += 1 * (height + spacing) -> & so on...
    ImGui::SetCursorPos(ImVec2(centered.x, centered.y
                + buttons.vec2.y + buttons.spacing));
    if (ImGui::Button("Change keybindings", buttons.vec2)) {
        m_gui_menu = GuiMenu::Keybindings;
        std::cout << "Entering change keybindings menu..." << std::endl;
    }

    ImGui::SetCursorPos(ImVec2(centered.x, centered.y
                + (2.f * (buttons.vec2.y + buttons.spacing))));
    if (ImGui::Button("Exit", buttons.vec2)) {
        request_pop_stack();
    }

    ImGui::End();
}

void SettingsState::change_keybindings()
{
    // open child gui menu
    // create local gui size & pos to be smaller than main menu
    sf::Vector2f gui_size = m_gui_size;
    sf::Vector2f gui_pos = m_gui_pos;
    // for each -0.1 size, +0.2 pos -> to center
    scale_vector2f(gui_size, 0.85f);
    scale_vector2f(gui_pos, 1.3f);
    ImGui::SetNextWindowPos(gui_pos, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(gui_size);
    // button dimensions & center
    ImButton buttons{{100.f, 25.f}, 5.f, 1.f};
    // don't want to center button!
    //sf::Vector2f centered = center_imbutton(gui_size, buttons);

    ImGui::Begin("Keybindings", &m_no_imclose, m_imflags);

    // don't want to center button!
    //ImGui::SetCursorPos(centered);

    // init empty char buf
    //char buf1[32] = "";
    //char buf2[32] = "";
    //char buf3[32] = "";
    //char buf4[32] = "";

    // TODO: problem solve input text, double input issue
    //if (ImGui::InputText("Move up", buf1, 2)) {
    if (ImGui::Button("Move up")) {
        m_assigning_keys = true;
        m_action = Player::Action::MoveUp;
    }
    //if (ImGui::InputText("Move down", buf2, 2)) {
    if (ImGui::Button("Move down")) {
        m_assigning_keys = true;
        m_action = Player::Action::MoveDown;
    }
    //if (ImGui::InputText("Move left", buf3, 2)) {
    if (ImGui::Button("Move left")) {
        m_assigning_keys = true;
        m_action = Player::Action::MoveLeft;
    }
    //if (ImGui::InputText("Move right", buf4, 2)) {
    if (ImGui::Button("Move right")) {
        m_assigning_keys = true;
        m_action = Player::Action::MoveRight;
    }
    if (ImGui::Button("Back", buttons.vec2))
        m_gui_menu = GuiMenu::Main;

    ImGui::End();
}

SettingsState::~SettingsState()
{
    // close gui upon state destruction
    //ImGui::End();
    //ImGui::SFML::Shutdown();
    std::cout << "Settings state has been destroyed!" << std::endl;
}
