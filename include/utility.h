#pragma once

//#define SFML_STATIC

#include <SFML/System/Vector2.hpp>

namespace sf {
    class Sprite;
    class Text;
}

struct ImButton {
    sf::Vector2f vec2{0.f, 0.f};
    float spacing = 0.f;
    float count = 0.f;
};

/**
 * Centers the origin of the sprite to the center of the sprite (as opposed to
 * the top left corner).
 * @param sf::Sprite& object
 * @return void return
 */
void center_origin(sf::Sprite& sprite);
void center_origin(sf::Text& text);
void scale_sprite(sf::Sprite& sprite);
void scale_vector2f(sf::Vector2f& vector2f, float scale);
sf::Vector2f center_imbutton(const sf::Vector2f& gui_size,
        const ImButton& button);
float to_degree(float radian);
float to_radian(float degree);
int random_int(int exclusive_max);
float length(sf::Vector2f vec2);
sf::Vector2f unit_vector(sf::Vector2f vec2);

// works with sf::Sprite & sf::Texture
//template <typename Sprite, typename Texture>
//void scale_to_sprite(const Sprite& sprite, Texture& texture);

/*
// change arr[0] to arr.x - to access arr with named members
template <std::size_t Index, typename T>
struct NamedArrayElement {
    char trash; // non-0 size for compiler - force minimal size, 1 byte
    // allows: double d = object.Member;
    operator T&() { return ((T*)(this)[Index]; }
    // allows: object.Member = 1.0;
    T& operator=(const T& rhs)
    {
        T &me = ((T*)(this))[Index];
        me = rhs;
        return rhs;
    }
    // allows: double *p = &object.Member;
    T* operator&() { return &((T*)(this))[Index]; }
    // allows: if (object.Member < 1.0)
    bool operator<(const T& rhs) { return ((T*)(this))[Index] < rhs; }
};
*/
