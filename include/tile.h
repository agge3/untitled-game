#pragma once

#include <SFML/Graphics/Rect.hpp>

class Tile {
public:
    // default - initializes pos and type
    Tile(int x, int y, int tile_type);
    // shows the tile
    // TODO: does arg need to be const?
    void render(sf::IntRect& camera);
    // get tile type
    int get_type() const;
    // get collision box
    // TODO: does return need to be const or &?
    const sf::IntRect& get_collision_box() const;
private:
    // attributes of the tile
    sf::IntRect m_box;
    // tile type
    int m_type;
};
