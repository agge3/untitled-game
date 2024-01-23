#include "s_pause.h"
#include "r_holders.h"
#include "utility.h"
#include "imguistyle.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <imgui-SFML.h>

#include <iostream>

PauseState::PauseState(StateStack& stack, Context context) :
    State(stack, context),
    m_window(*get_context().window),
    m_background_sprite(), // in case want background
    m_paused_text(),
    m_instruction_text()
{
    // set view to full size of render window for pause screen
    m_window.setView(m_window.getDefaultView());

    // get res from existing context
    sf::Font& font = context.fonts->get(Fonts::Main);
    sf::Vector2f view_size = context.window->getView().getSize();

    // pause screen/menu...
    m_paused_text.setFont(font);
    m_paused_text.setString("Paused...");
    m_paused_text.setCharacterSize(70);
    center_origin(m_paused_text);
    m_paused_text.setPosition(0.5f * view_size.x, 0.4f * view_size.y);

    m_instruction_text.setFont(font);
    m_instruction_text.setString(
            "(Press Backspace to return to the main menu)");
    center_origin(m_instruction_text);
    m_instruction_text.setPosition(0.5f * view_size.x, 0.6f * view_size.y);

    // init gui, style, & options
    if (!ImGui::SFML::Init(m_window) == -1) // -1 is init failure
        throw std::runtime_error("Pause GUI failed to initialize!");
    ImGui::SetupImGuiStyle();
}

void PauseState::draw()
{
    // get &window from context & set full screen (default view)
    sf::RenderWindow& window = *get_context().window;
    window.setView(window.getDefaultView());

    // opacity layer over paused game
    sf::RectangleShape background_shape;
    // rgba (0, 0, 0, alpha val for opacity)
    background_shape.setFillColor(sf::Color(0, 0, 0, 150));
    // size of layer = size of window
    background_shape.setSize(window.getView().getSize());

    // draw pause screen assets
    window.draw(background_shape);
    window.draw(m_paused_text);
    window.draw(m_instruction_text);
    ImGui::SFML::Render(m_window); // frame ready -> now render in render loop
}

/**
 * @param sf::Time delta_time (game delta time).
 * @return Returns false to keep previous state NOT updating.
 * @note Used to "pause" previous state, like the game state.
 */
bool PauseState::update(sf::Time delta_time)
{
    ImGui::SFML::Update(m_window, delta_time); // new frame - begin
    gui_frame(); // frame logic
    ImGui::EndFrame(); // end frame - end
    return false;
}

bool PauseState::handle_event(const sf::Event& event)
{
    ImGui::SFML::ProcessEvent(m_window, event);

    /* old keyboard input
    if (event.type != sf::Event::KeyPressed)
        return false; // do nothing if no key is pressed
    if (event.key.code == sf::Keyboard::Escape) {
        // exit pause screen, pop off stack and return to game (top of stack)
        request_pop_stack();
    }
    if (event.key.code == sf::Keyboard::BackSpace) {
        // clear all states and push menu to stack (return to main menu)
        request_clear_state();
        request_push_stack(States::Menu);
    }
    */

    return false;
}

void PauseState::gui_frame() {
    // (left) gui_pos + gui_size + (right) gui_pos = 1.0, to center gui window
    sf::Vector2f gui_size =
        static_cast<sf::Vector2f>(m_window.getView().getSize());
    sf::Vector2f gui_pos = gui_size;
    /* std::cout << "View size: " << gui_size.x << "x*" << gui_size.y << "y\n"; */
    scale_vector2f(gui_pos, 0.2f);
    /* std::cout << "GUI position: " << gui_pos.x << "x*" << gui_pos.y
        << "y\n"; */
    ImGui::SetNextWindowPos(gui_pos);
    scale_vector2f(gui_size, 0.6f);
    /* std::cout << "GUI size: " << gui_size.x << "x*" << gui_size.y << "y"
        << std::endl; */
    ImGui::SetNextWindowSize(gui_size);

    // imgui flags for window init
    bool no_close_button = true;
    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoSavedSettings;

    ImGui::Begin("Pause", &no_close_button, flags);

    // button dimensions
    ImButton buttons{{200.f, 25.f}, 5.f, 3.f};
    sf::Vector2f centered = center_imbutton(gui_size, buttons);

    // button layout
    ImGui::SetCursorPos(centered); // set starting pos of button
    if (ImGui::Button("Return to main menu", buttons.vec2)) {
        request_clear_state();
        request_push_stack(States::Menu);
    }
    // next button y centered += 1 * (height + spacing)
    ImGui::SetCursorPos(ImVec2(centered.x, centered.y
                + buttons.vec2.y + buttons.spacing));
    if (ImGui::Button("Return to game", buttons.vec2)) {
        request_pop_stack();
    }
    // next button y centered += 2 * (height + spacing) -> & so on...
    ImGui::SetCursorPos(ImVec2(centered.x, centered.y
                + (2.f * (buttons.vec2.y + buttons.spacing))));
    if (ImGui::Button("Settings", buttons.vec2)) {
        request_push_stack(States::Settings);
    }

    ImGui::End();
}

PauseState::~PauseState()
{
    std::cout << "Pause state has been destroyed!" << std::endl;
}
