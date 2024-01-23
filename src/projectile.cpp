#include "projectile.h"
#include "data_tables.h"
#include "utility.h"
#include "r_holders.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>
#include <cassert>

// anon namespace to prevent naming conflicts - local TABLE for entity
namespace {
    const std::vector<ProjectileData> TABLE = initialize_projectile_data();
}

Projectile::Projectile(Type type, const TextureHolder& textures) :
    Entity(1),
    m_type(type),
    m_sprite(textures.get(TABLE[type].texture)),
    m_target_direction()
{
    center_origin(m_sprite);
}

void Projectile::guide_torwards(sf::Vector2f position)
{
    assert(is_guided());
    m_target_direction = unit_vector(position - get_world_position());
}

bool Projectile::is_guided() const {
    return m_type == Projectile::PlayerFire;
}

void Projectile::update_current(sf::Time delta_time, CommandQueue& commands)
{
    /* TODO: implement/understand guiding and math + calc
    if (is_guided()) {
        const float approach_rate = 125.f;
        sf::Vector2f new_velocity = unit_vector2f(approach_rate
                * delta_timfe.asSeconds() * m_target_direction + get_velocity());
        new_velocity *= get_max_speed();
        float angle = std::atan2(new_velocity.y, new_velocity.x);

        set_rotation(to_degree(angle) + 90.f);
        set_velocity(new_velocity);
    }
    */
    Entity::update_current(delta_time, commands);
}

void Projectile::draw_current(sf::RenderTarget& target, sf::RenderStates states)
    const
{
    target.draw(m_sprite, states);
}

unsigned int Projectile::get_category() const
{
    if (m_type == Type::EnemyFire)
        return Category::EnemyProjectile;
    else
        return Category::PlayerProjectile;
}

sf::FloatRect Projectile::get_bounding_rect() const
{
    return get_world_transform().transformRect(m_sprite.getGlobalBounds());
}

/**
 * Gets the max speed of the projectile from its data table.
 * @return Float max speed.
 * @note Can be used for both x and y velocity!
 */
float Projectile::get_max_speed() const
{
    return TABLE[m_type].speed;
}

float Projectile::get_damage() const
{
    return TABLE[m_type].damage;
}
