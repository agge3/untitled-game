#pragma once

#include "scene_node.h"
/// Include command_queue.h in entity.h because all derived classes of Entity
/// use CommandQueue.
#include "command_queue.h"

/**
 * @class Entity
 * Entity inherits from SceneNode, because all entities are nodes on the scene
 * graph. Derived classes of Entity inherit its base attributes, meaning all
 * entities in-game share similar base attributes. Derived classes can implement
 * their own unique attributes.
 */
class Entity : public SceneNode {
public:
    /// All entities have velocity and hitpoints.
    explicit Entity(float hitpoints) : m_velocity(), m_hitpoints(hitpoints) {}
    void heal(float hitpoints);
    void damage(float hitpoints);
    void destroy();

    float get_hitpoints() const;
    bool is_destroyed() const;

    void set_velocity(sf::Vector2f velocity);
    void set_velocity(float vx, float vy);
    void accelerate(sf::Vector2f velocity);
    void accelerate(float vx, float vy);
    sf::Vector2f get_velocity() const;
protected:
    /// Protected for derived class(es) to access directly.
    /// Virtual fn overwritten in derived class(es) implementation.
    virtual void update_current(sf::Time delta_time, CommandQueue& commands);
private:
    float m_hitpoints;
    sf::Vector2f m_velocity;
};
