#include "utility.h"
#include "conf.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <imgui.h>

#include <cmath>
#include <cassert>
#include <iostream>
#include <numbers>
#include <ctime>
#include <random>

/// Anonymous namespace to create local random engine to be used.
namespace {
    std::default_random_engine create_random_engine()
    {
        /// Create seed based on current std::time.
        auto seed = static_cast<unsigned long>(std::time(nullptr));
        /// Use std::default_random_engine based on seed.
        return std::default_random_engine(seed);
    }
    /// RandomEngine is interface to use random engine.
    auto RandomEngine = create_random_engine();
}

void center_origin(sf::Sprite& sprite)
{
	sf::FloatRect bounds = sprite.getLocalBounds();
	sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f),
            std::floor(bounds.top + bounds.height / 2.f));
}

void center_origin(sf::Text& text)
{
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(std::floor(bounds.left + bounds.width / 2.f),
            std::floor(bounds.top + bounds.height / 2.f));
}

void scale_sprite(sf::Sprite& sprite)
{
    // store float vector size for readability
    sf::Vector2f size = sprite.getLocalBounds().getSize();

    // console output for debugging
    std::cout << "Sprite size: " << size.x << "x*" << size.y << "y\n"
        << "Resolution size: " << static_cast<float>(conf::RESOLUTION_X) << "x*"
        << static_cast<float>(conf::RESOLUTION_Y) << "y" << std::endl;

    if (size.x > static_cast<float>(conf::RESOLUTION_X)
            || size.y > static_cast<float>(conf::RESOLUTION_Y)) {
        float x_percent = static_cast<float>(conf::RESOLUTION_X) / size.x;
        float y_percent = static_cast<float>(conf::RESOLUTION_Y) / size.y;

        assert(x_percent < 1 || y_percent < 1);

        // maintain correct aspect ratio when resizing, resize axis by same %
        if (x_percent != y_percent)
            x_percent > y_percent ? y_percent = x_percent : x_percent = y_percent;

        // print scale factor for debugging
        std::cout << "Scale factor: " << x_percent << std::endl;

        sprite.scale(x_percent, y_percent);
    }
}

void scale_vector2f(sf::Vector2f& vector2f, float scale)
{
    vector2f.x *= scale;
    vector2f.y *= scale;
}

sf::Vector2f center_imbutton(const sf::Vector2f& gui_size,
        const ImButton& button)
{
    // centered x pos = (total x / 2) - (width / 2)
    float x_pos = (gui_size.x / 2.f) - (button.vec2.x / 2.f);
    // starting y pos: center = (total y / 2)
    // offset = (count * height) + ((count - 1) * spacing)) / 2
    // centered = center - offset
    float y_pos = (gui_size.y / 2.f) - (((button.count * button.vec2.y)
                + ((button.count - 1.f) * button.spacing)) / 2.f);
    // if button count = 1, same as x calc
    if (button.count < 2)
        y_pos = (gui_size.y / 2.f) - (button.vec2.y / 2);

    return sf::Vector2f{x_pos, y_pos};
}

/// Converts a radian to a degree.
float to_degree(float radian)
{
    /// Uses std::numbers::pi_v<T> - CXX20.
    // 180 divided by pi, times radian
    return (180.f / std::numbers::pi_v<float> * radian);
}

/// Converts a degree to a radian.
float to_radian(float degree)
{
    /// Uses std::numbers::pi_v<T> - CXX20.
    // pi divided by 180, times degree
    return (std::numbers::pi_v<float> / 180.f * degree);
}

/**
 * Returns a random int.
 * @param Max int.
 * @return Int exclusive of max will be returned.
 */
int random_int(int exclusive_max)
{
    std::uniform_int_distribution<> distr(0, exclusive_max -1);
    return distr(RandomEngine);
}

/// Returns float length of a float Vector2.
float length(sf::Vector2f vec2)
{
    return std::sqrt((vec2.x * vec2.x) + (vec2.y * vec2.y));
}

/// Returns a unit vector - a vector with length = 1.
sf::Vector2f unit_vector(sf::Vector2f vec2)
{
    // vector cannot be (0, 0)!
    assert(vec2 != sf::Vector2f(0.f, 0.f));
    // by dividing by length, set length to constant of 1
    return (vec2 / length(vec2));
}
