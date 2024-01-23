#include "debug.h"
#include "conf.h"

#include <SFML/Graphics/Text.hpp>

#include <iostream>

using namespace conf;

void Debug::print_window() const
{
    std::cout << "Depth bits: " << CONTEXT_SETTINGS.depthBits << "\n"
        << "Stencil bits: " << CONTEXT_SETTINGS.stencilBits << "\n"
        << "Antialiasing level: " << CONTEXT_SETTINGS.antialiasingLevel << "\n"
        << "Major version: " << CONTEXT_SETTINGS.majorVersion << "\n"
        << "Minor verson: " << CONTEXT_SETTINGS.minorVersion << "\n"
        << "Attribute flags: " << CONTEXT_SETTINGS.attributeFlags << "\n"
        << "sRGB capable: " << CONTEXT_SETTINGS.sRgbCapable << std::endl;
}

void Debug::print_video_modes() const
{
    for (std::size_t i{0}; i < VIDEO_MODES.size(); ++i) {
        std::cout << "Mode " << i << ": "
            << VIDEO_MODES[i].width << "x*" << VIDEO_MODES[i].height << "y "
            << VIDEO_MODES[i].bitsPerPixel << "bpp" << std::endl;
    }
}

void Debug::print_text(const sf::Text& text) const
{
    std::cout << "Text: \"" << text.getString().toAnsiString() << "\"\n"
        << "Font: " << text.getFont()->getInfo().family << "\n"
        << "Size: " << text.getCharacterSize() << "\n"
        << "Style: " << text.getStyle() << "\n"
        << "Color: " << text.getFillColor().toInteger() << std::endl;
}
