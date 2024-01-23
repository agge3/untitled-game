#include "app.h"
#include "state.h"
#include "s_ids.h"
#include "s_title.h"
#include "s_menu.h"
#include "s_game.h"
#include "s_pause.h"
#include "s_loading.h"
#include "s_settings.h"
#include "utility.h"
#include "conf.h"
#include "debug.h"

#include <SFML/System.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include <stdexcept>
#include <iostream>

using namespace conf;

Application::Application() :
    // default resolution, title, & window style
    m_window(sf::VideoMode(RESOLUTION_X, RESOLUTION_Y, 16), TITLE,
            sf::Style::Close),
    m_textures(),
    m_player(),
    // reused context loading between states
    m_state_stack(State::Context(m_window, m_textures, m_fonts, m_player)),
    m_debug()
{
    // enable v-sync
    m_window.setVerticalSyncEnabled(VSYNC_TRUE);
    // disable holding a key repeats action
    m_window.setKeyRepeatEnabled(false);

    // assign openGL context & get video modes
    assign_context_settings();
    store_video_modes();

    // debug - print context settings & video modes
    m_debug.print_window();
    m_debug.print_video_modes();

    // init GUI for use throughout states
    /*if (ImGui::SFML::Init(m_window) == -1) // -1 is return of init failure
        throw std::runtime_error("Settings GUI failed to initialize!");
    else
        std::cout << "ImGui initialized!" << std::endl;*/

    // load main font
    m_fonts.load(Fonts::Main, "fonts/Hack-Regular.ttf");
    // load title screen
    m_textures.load(Textures::TitleScreen, "textures/title/welcome_to_gaem.png");

    register_states();
    m_state_stack.push_state(States::Title);
}

void Application::run()
{
    // debug if loop is entered
    if (m_debug.debug_counter == 0)
        ++m_debug.debug_counter;
    else if (m_debug.debug_counter == 1)
        std::cout << "Entering application run loop" << std::endl;

    sf::Clock clock; // game clock
    sf::Time time_since_last_update = sf::Time::Zero;

    // game poll, outer game loop -> variable rendering (as fast as possible)
    // game loop: (1) process_input, (2) update, (3) render
    // state loop: (1) handle_event, (2) update, (3) draw
    while (m_window.isOpen()) {
        // setup to adjust for timestep
        sf::Time elapsed_time = clock.restart();
        time_since_last_update += elapsed_time;

        // wait for framerate to match expected - fixes delta time issues
        if (time_since_last_update > TIME_PER_FRAME) {
            // game logic loop, while framerate matches expected
            // do first since cond is already checked
            do {
            time_since_last_update -= TIME_PER_FRAME;
            // recieve input and put into command queue
            process_input();
            // update in inner loop
            update(TIME_PER_FRAME);
            // confirm further loops by checking cond again
            } while (time_since_last_update > TIME_PER_FRAME);
            // NOTE: render needs to be guarded by if cond or else frames out of sync
            // render after main loop, everything is prepared and ready to render
            render();
        // sleep until framerate matches expected
        } else {
            sf::sleep(TIME_PER_FRAME - time_since_last_update);
        }
    }
    // close gui
    //ImGui::SFML::Shutdown();
}
// article, timestep common isssue -> https://gafferongames.com/post/fix_your_timestep/

void Application::process_input()
{
    sf::Event event;
    while (m_window.pollEvent(event)) {
        m_state_stack.handle_event(event);
        if (event.type == sf::Event::Closed)
            m_window.close();
    }
}

void Application::update(sf::Time delta_time) {
    //ImGui::SFML::Update(m_window, delta_time);
    m_state_stack.update(delta_time);
}

void Application::render()
{
    // clear window
    m_window.clear();
    // redraw window (based on state)
    m_state_stack.draw();
    // default view and display buffered window
    //m_window.setView(m_window.getDefaultView());
    //ImGui::SFML::Render(m_window);
    //ImGui::SFML::Render(m_window);
    m_window.display();
}

void Application::register_states()
{
    m_state_stack.register_state<TitleState>(States::Title);
    m_state_stack.register_state<MenuState>(States::Menu);
    m_state_stack.register_state<GameState>(States::Game);
    m_state_stack.register_state<PauseState>(States::Pause);
    m_state_stack.register_state<LoadingState>(States::Loading);
    m_state_stack.register_state<SettingsState>(States::Settings);
}
