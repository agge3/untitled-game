#pragma once

//#define SFML_STATIC

#include <cstddef>

namespace sf {
    class Text;
}

class Debug {
public:
    std::size_t debug_counter{0};

    void print_window() const;
    void print_video_modes() const;
    void print_text(const sf::Text& text) const;

    bool m_is_drawing_bounding_rect;
};
