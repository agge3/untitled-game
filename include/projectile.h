#pragma once

#include "entity.h"
#include "r_ids.h"
#include "r_holders.h"

#include <SFML/Graphics/Sprite.hpp>

class Projectile : public Entity {
public:
    /**
     * @enum Type
     * Type of projectile entity.
     */
    enum Type : unsigned int {
       PlayerFire,
       NeutralFire,
       EnemyFire,
       TypeCount,
    };

    Projectile(Type type, const TextureHolder& textures);
    void guide_torwards(sf::Vector2f position);
    bool is_guided() const;
    virtual unsigned int get_category() const;
    virtual sf::FloatRect get_bounding_rect() const;
    float get_max_speed() const;
    float get_damage() const;
private:
    virtual void update_current(sf::Time delta_time, CommandQueue& commands);
    virtual void draw_current(sf::RenderTarget& target, sf::RenderStates states)
        const;

    Type m_type;
    sf::Sprite m_sprite;
    sf::Vector2f m_target_direction;
};
