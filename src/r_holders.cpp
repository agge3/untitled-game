#include "r_holders.h"

void TextureHolder::load(Textures::ID id, const std::string& filename)
{
    // create texture
    std::unique_ptr<sf::Texture> texture(new sf::Texture());
    // load texture and evaluate if load is successful
    if (!texture->loadFromFile(filename))
        throw std::runtime_error("TextureHolder::load - Failed to load "
                + filename);
    // if load is successful, insert into texture map
    insert_texture(id, std::move(texture));
}

template <typename Optional>
void TextureHolder::load(Textures::ID id, const std::string& filename,
        const Optional& option)
{
    // same as previous, including optional parameter

    // create texture
    std::unique_ptr<sf::Texture> texture(new sf::Texture());
    // load texture and evaluate if load is successful
    if (!texture->loadFromFile(filename, option))
        throw std::runtime_error("TextureHolder::load - Failed to load "
                + filename);
    // if load is successful, insert into texture map
    insert_texture(id, std::move(texture));
}

sf::Texture& TextureHolder::get(Textures::ID id)
{
    // if found, will recieve id and confirm. if not, recieve end() & assert
    auto found = m_texture_map.find(id);
    assert(found != m_texture_map.end());
    // texture is second member in the map
    return *found->second;
}

const sf::Texture& TextureHolder::get(Textures::ID id) const
{
    // same as previous implementation

    // if found, will recieve id and confirm. if not, recieve end() & assert
    auto found = m_texture_map.find(id);
    assert(found != m_texture_map.end());
    // texture is second member in the map
    return *found->second;
}

void TextureHolder::insert_texture(Textures::ID id,
        std::unique_ptr<sf::Texture> texture)
{
    auto inserted = m_texture_map.insert(std::make_pair(id,
                std::move(texture)));
    // returns true in second member if successful, check success
    assert(inserted.second);
}

void FontHolder::load(Fonts::ID id, const std::string& filename)
{
    // create font
    std::unique_ptr<sf::Font> font(new sf::Font());
    // load font and evaluate if load is successful
    if (!font->loadFromFile(filename))
        throw std::runtime_error("FontHolder::load - Failed to load "
                + filename);
    // if load is successful, insert into texture map
    insert_font(id, std::move(font));
}

sf::Font& FontHolder::get(Fonts::ID id)
{
    // if found, will recieve id and confirm. if not, recieve end() & assert
    auto found = m_font_map.find(id);
    assert(found != m_font_map.end());
    // texture is second member in the map
    return *found->second;
}

const sf::Font& FontHolder::get(Fonts::ID id) const
{
    // same as previous implementation

    // if found, will recieve id and confirm. if not, recieve end() & assert
    auto found = m_font_map.find(id);
    assert(found != m_font_map.end());
    // texture is second member in the map
    return *found->second;
}

void FontHolder::insert_font(Fonts::ID id,
        std::unique_ptr<sf::Font> font)
{
    auto inserted = m_font_map.insert(std::make_pair(id,
                std::move(font)));
    // returns true in second member if successful, check success
    assert(inserted.second);
}
