#include "pickup.h"
#include "data_tables.h"
#include "category.h"
#include "command_queue.h"
#include "utility.h"
#include "r_holders.h"

#include <SFML/Graphics/RenderTarget.hpp>

/// Local TABLE in anonymous namespace to prevent naming conflicts amongst entities.
namespace {
    const std::vector<PickupData> TABLE = initialize_pickup_data();
}

Pickup::Pickup(Type type, const TextureHolder& textures) :
    Entity(1),
    m_type(type),
    m_sprite(textures.get(TABLE[type].texture))
{
    /// Default constructor centers origin of sprite.
    center_origin(m_sprite);
}

unsigned int Pickup::get_category() const
{
    return Category::Pickup;
}

/**
 * Gets the bounding rectangle of the pickup.
 * @return sf::FloatRect that is the bounding rectangle.
 */
sf::FloatRect Pickup::get_bounding_rect() const
{
    // bounds of the sprite, respecting its transform
    return get_world_transform().transformRect(m_sprite.getGlobalBounds());
}

void Pickup::apply(Creature& player) const
{
    /// Lookup TABLE by type & apply action to player.
    TABLE[m_type].action(player);
}

void Pickup::draw_current(sf::RenderTarget& target, sf::RenderStates states)
    const
{
    target.draw(m_sprite, states);
}
