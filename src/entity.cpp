//#define SFML_STATIC

#include "entity.h"

void Entity::heal(float hitpoints)
{
    /// Make sure hitpoints are greater than zero.
    assert(hitpoints > 0);
    /// Add hitpoints to be healed to hitpoints.
    m_hitpoints += hitpoints;
}

void Entity::damage(float hitpoints)
{
    /// Make sure hitpoints are greater than zero.
    assert(hitpoints > 0);
    /// Subtract damage from hitpoints.
    m_hitpoints -= hitpoints;
}

/// Sets hitpoints to zero.
void Entity::destroy()
{
    m_hitpoints = 0;
}

/**
 * @return Returns the entity's current hitpoints.
 */
float Entity::get_hitpoints() const
{
    return m_hitpoints;
}

/**
 * Determines if Entity is destroyed by checking if hitpoints are greater than
 * zero.
 * @return True if destroyed, false if not.
 */
bool Entity::is_destroyed() const
{
    return m_hitpoints <= 0;
}

void Entity::set_velocity(sf::Vector2f velocity)
{
    m_velocity = velocity;
}

void Entity::set_velocity(float vx, float vy)
{
    m_velocity.x = vx;
    m_velocity.y = vy;
}

sf::Vector2f Entity::get_velocity() const
{
    return m_velocity;
}

void Entity::accelerate(sf::Vector2f velocity)
{
    m_velocity += velocity;
}

void Entity::accelerate(float vx, float vy)

{
    m_velocity.x += vy;
    m_velocity.y += vy;
}

/**
 * @note Overwrite update_current() in derived class(es) to add further
 * functionality.
 * @return Returns how much the entity has moved, based on delta time elapsed.
 */
void Entity::update_current(sf::Time delta_time, CommandQueue& commands)
{
    /// Move entity based on velocity and delta time elapsed.
    move(m_velocity * delta_time.asSeconds());
}
