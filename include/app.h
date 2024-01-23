#pragma once

//#define SFML_STATIC

#include "r_holders.h"
#include "r_ids.h"
#include "s_stack.h"
#include "player.h"
#include "debug.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Application {
public:
    Application();

    void run();
private:
    void process_input();
    void update(sf::Time delta_time);
    void render();
    void register_states();

    sf::RenderWindow m_window;
    TextureHolder m_textures;
    FontHolder m_fonts;
    Player m_player;
    StateStack m_state_stack;
    Debug m_debug;
};
