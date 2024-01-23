#include "conf.h"

using namespace conf;

void conf::assign_context_settings()
{
    CONTEXT_SETTINGS.depthBits = 24;
    CONTEXT_SETTINGS.stencilBits = 8;
    CONTEXT_SETTINGS.antialiasingLevel = 4;
    CONTEXT_SETTINGS.majorVersion = 4;
    CONTEXT_SETTINGS.minorVersion = 0;
}

void conf::store_video_modes()
{
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    VIDEO_MODES = mode.getFullscreenModes();
}
