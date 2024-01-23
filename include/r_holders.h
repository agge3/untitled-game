#pragma once

//#define SFML_STATIC
//#define NDEBUG

#include "r_ids.h"

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

class TextureHolder {
public:
    void load(Textures::ID id, const std::string& filename);

    // overloaded load, T can be either sf::Shader::Type or const std::string&
    // for vertex & fragment shader
    // also allows additional argument for loadFromFile(), const IntRect& area
    template <typename Optional>
    void load(Textures::ID id, const std::string& filename, const Optional& option);

    sf::Texture& get(Textures::ID id);
    const sf::Texture& get(Textures::ID id) const;
private:
    void insert_texture(Textures::ID id, std::unique_ptr<sf::Texture> texture);

    std::map<Textures::ID, std::unique_ptr<sf::Texture>> m_texture_map;
};

// same implementation as texture holder - recreate as necessary
class FontHolder {
public:
    void load(Fonts::ID id, const std::string& filename);
    sf::Font& get(Fonts::ID id);
    const sf::Font& get(Fonts::ID id) const;
private:
    void insert_font(Fonts::ID id, std::unique_ptr<sf::Font> font);
    std::map<Fonts::ID, std::unique_ptr<sf::Font>> m_font_map;
};
