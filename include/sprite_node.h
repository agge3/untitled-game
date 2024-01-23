#pragma once

#include "scene_node.h"

#include <SFML/Graphics/Sprite.hpp>

// manipulate the sprite object manipulates the sprite. don't have to
// redraw sprite, can be accessed through sprite node
class SpriteNode : public SceneNode {
public:
    explicit SpriteNode(const sf::Texture& texture);
    SpriteNode(const sf::Texture& texture, const sf::IntRect& texture_rect);
private:
    virtual void draw_current(sf::RenderTarget& target, sf::RenderStates states)
        const;
    // sf::Sprite prepared at startup and not touched again
    sf::Sprite m_sprite;
};
