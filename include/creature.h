#pragma once

#include "entity.h"
#include "r_holders.h"
#include "text_node.h"
#include "command.h"
#include "projectile.h"
#include "pickup.h"

#include <SFML/Graphics/Sprite.hpp>

#include <ostream>

class Creature : public Entity {
public:
    /**
     * @enum Type
     * Type of Creature Entity.
     */
    enum Type : unsigned int {
        Player,
        Bunny,
        Bear,
        TypeCount,
    };

    /// Default constructor: type, textures, and fonts.
    Creature(Type type, const TextureHolder& textures, const FontHolder& fonts);

    /// Must be friend fn to properly access Creature::Type.
    friend std::ostream& operator<<(std::ostream& out, const Creature::Type type);

    virtual unsigned int get_category() const;
    virtual sf::FloatRect get_bounding_rect() const;
    virtual bool is_marked_for_removal() const;
    bool is_allied() const;
    float get_max_speed() const;
    void attack();
private:
    virtual void draw_current(sf::RenderTarget& target, sf::RenderStates states)
        const;
    virtual void update_current(sf::Time delta_time, CommandQueue& commands);
    void update_pathing(sf::Time delta_time);
    void check_pickup_drop(CommandQueue& commands);
    void check_projectile_launch(sf::Time delta_time, CommandQueue& commands);
    void create_projectile(SceneNode& node, Projectile::Type type,
            float x_offset, float y_offset, const TextureHolder& textures) const;
    void create_pickup(SceneNode& node, const TextureHolder& textures) const;
    void update_texts();

    Type m_type;
    sf::Sprite m_sprite;
    Command m_attack_command;
    sf::Time m_attack_countdown;
    bool m_is_attacking;
    float m_attack_rate;
    bool m_is_marked_for_removal;
    Command m_drop_pickup_command;
    float m_travelled_distance;
    std::size_t m_direction_index;
    TextNode* m_health_display;
};
