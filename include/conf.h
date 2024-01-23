#ifndef CONF_H
#define CONF_H

//#define SFML_STATIC

#include <SFML/System/Time.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <string>
#include <vector>

namespace conf {
    // stored in static mem to always be allocated/available/persist
    // TODO: allow changing in ui
    static std::string TITLE = "untitled_game";
    static unsigned int RESOLUTION_X = 1280;
    static unsigned int RESOLUTION_Y = 720;
    static sf::Time TIME_PER_FRAME = sf::seconds(1.f / 60.f); // 60 fps
    static bool VSYNC_TRUE = true;

    // info about OpenGL context
    static sf::ContextSettings CONTEXT_SETTINGS;
    static std::vector<sf::VideoMode> VIDEO_MODES;

    void assign_context_settings();
    void store_video_modes();
}

#endif
