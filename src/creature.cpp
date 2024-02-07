//#define SFML_STATIC

#include "creature.h"
#include "data_tables.h"
#include "utility.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>
#include <iostream>
#include <string>
#include <stdexcept>

/// Anonymous namespace to avoid name collisions in other files - store Creature
/// data TABLE local to Creature.
namespace {
    const std::vector<CreatureData> TABLE = initialize_creature_data();
}

Creature::Creature(Type type, const TextureHolder& textures,
        const FontHolder& fonts) :
    Entity(TABLE[type].hitpoints),
    m_type(type),
    m_sprite(textures.get(TABLE[type].texture)),
    m_attack_command(),
    m_attack_countdown(sf::Time::Zero),
    m_is_attacking(false),
    m_is_marked_for_removal(false),
    m_drop_pickup_command(),
    m_travelled_distance(0.f),
    m_direction_index(0),
    m_health_display(nullptr)
{
    center_origin(m_sprite);

    /// Attack category and action is initialized in default constructor for
    /// direct access to &texture, ready for future use.
    m_attack_command.category = Category::SceneGroundLayer;
    // capture command & &textures -> create projectile for action
    m_attack_command.action = [this, &textures] (SceneNode& node, sf::Time) {
        create_projectile(node, Projectile::PlayerFire, 0.f, 0.5f, textures);
    };
    m_drop_pickup_command.category = Category::SceneGroundLayer;
    m_drop_pickup_command.action = [this, &textures] (SceneNode& node, sf::Time) {
        create_pickup(node, textures);
    };

    /** @brief Smart pointer to TextNode on the heap is initialized in default
     * constructor, for health display (and other text info...).
     * @note TextNode is suspect for errors. Has exception handling, is wrapped
     * in try-block. */
    // create text node & attach to creature - health display
    try {
        std::unique_ptr<TextNode> health_display(new TextNode(fonts, ""));
        m_health_display = health_display.get(); // mem ptr that points to node
        attach_child(std::move(health_display));
        // print success to match expected text nodes with expected creatures
        std::cout << "Text node initialized\n";
    } catch (std::exception& e) {
        std::cerr << "\nexception: " << e.what() << std::endl;
    }

    /// @todo Implement specific text info for player.
    //if (get_category() == Player) {
        // do special things for player
    //}

    // print success to match expected text updates with expected creatures
    std::cout << "Text for creature updated\n";
    update_texts();
}

/**
 * Overloaded insertion operator to print Creature::Type as std::string.
 * @return Returns ostream& of Creature::Type (as std::string).
 * @note Default is empty std::string.
 */
std::ostream& operator<<(std::ostream& out, const Creature::Type type)
{
    switch (type) {
    case Creature::Type::Player:
        out << "Player";
        break;
    case Creature::Type::Bunny:
        out << "Bunny";
        break;
    case Creature::Type::Bear:
        out << "Bear";
        break;
    case Creature::Type::TypeCount:
        out << std::to_string(Creature::Type::TypeCount);
        break;
    default:
        out << "";
        break;
    }
    return out;
}

void Creature::draw_current(sf::RenderTarget& target, sf::RenderStates states)
    const
{
    target.draw(m_sprite, states);
}

/**
 * Update the current Creature.
 */
void Creature::update_current(sf::Time dt, CommandQueue& commands)
{
    /** @note Checks if Creature is_destroyed() first, if is_destroyed() RNG
     * check_pickup_drop() and is_marked_for_removal(). */
    if (is_destroyed()) {
        check_pickup_drop(commands);
        m_is_marked_for_removal = true;
    } else {
        /** @brief check_projectile_launch() to check if attack(s) should be
         * updated. */
        check_projectile_launch(dt, commands);
        /** @brief Update Creature pathing and apply velocity. */
        update_pathing(dt);
        Entity::update_current(dt, commands);
        /** @brief Update Creature texts. */
        update_texts();
    }
}

void Creature::update_pathing(sf::Time dt)
{
    // enemy creature - pathing
    // const ref to directions defined in data table
    const std::vector<Direction>& DIRECTIONS = TABLE[m_type].directions;
    if (!DIRECTIONS.empty()) { // do nothing if no directions
        // wait for travelled distance to be greater than defined pathing
        if (m_travelled_distance > DIRECTIONS[m_direction_index].distance) {
            // using modulo allows restarting and cycling through directions vec
            m_direction_index = (m_direction_index + 1) % DIRECTIONS.size();
            // reset distance travelled to complete next pathing instr
            m_travelled_distance = 0.f;
        }

        // compute velocity from direction
        /* NOTE: if the distance to travel is no multiple of the creature's
        speed, the creature will move further than intended. */
        float radians = to_radian(DIRECTIONS[m_direction_index].angle);
        std::cout << "Radians of creature: " << radians << "\n";
        // velocity for x = speed * cos(radians)
        float vx = get_max_speed() * std::cos(radians);
        // velocity for y = speed * sin(radians)
        float vy = get_max_speed() * std::sin(radians);
        set_velocity(vx, vy);
        std::cout << "Velocity of creature: " << vx << "x*" << vy << "y"
            << std::endl;
        // distance travelled = speed * time
        m_travelled_distance += get_max_speed() * dt.asSeconds();
    }
}

/**
 * RNG to determine if NPC should drop Pickup.
 * @see random_int() for RNG implementation.
 */
void Creature::check_pickup_drop(CommandQueue& commands)
{
    /** @attention Enemy(ies) have 1/3 chance to drop Pickup. */
    if (!is_allied() && random_int(3) == 0) {
        commands.push(m_drop_pickup_command);
    }
}

/**
 * Assign correct Texture to ID of Creature.
 * @note Default Texture is Textures::Player.
 */
Textures::ID to_texture_id(Creature::Type type)
{
    switch (type) {
    case Creature::Player:
        return Textures::Player;
    case Creature::Bunny:
        return Textures::Bunny;
    case Creature::Bear:
        return Textures::Bear;
    }
    // If Texture not found, default Texture is Textures::Player
    return Textures::Player;
}

/**
 * Returns Category of Creature.
 * @return unsigned int for enum Category::Type.
 * @note Default category is EnemyNPC.
 * @remark Overwrites inherited fn get_category(), to give new behavior for
 * Creature.
 */
unsigned int Creature::get_category() const
{
    /// Switch type of Creature to get Category.
    switch (m_type) {
    /// e.g. Set Creature::Player to Category::Player.
    case Player:
        return Category::Player;
    default:
        return Category::EnemyNpc;
    }
}

/**
 * @return Returns the bounding rectangle of the creature.
 */
sf::FloatRect Creature::get_bounding_rect() const
{
    /// Uses the bounds of the sprite and its current transformation to
    /// determine bounding rectangle.
    return get_world_transform().transformRect(m_sprite.getGlobalBounds());
}

/// Uses member variable flag to determine if marked for removal or not.
bool Creature::is_marked_for_removal() const
{
    return m_is_marked_for_removal;
}

void Creature::update_texts()
{
    // catting str with '+'...?
    m_health_display->set_string(std::to_string(get_hitpoints()) + " HP");
    // print success to make sure this is only done once!
    std::cout << "Update texts: Health display text set ... success!\n"
        // and shows the correct string...
        << "Text: " << std::to_string(get_hitpoints()) << " HP\n";
    m_health_display->setPosition(0.f, 50.f);
    // -rotation negates any rotation of creature and keeps text upright
    m_health_display->setRotation(-getRotation());
}

void Creature::check_projectile_launch(sf::Time delta_time,
        CommandQueue& commands)
{
    /// Enemies try to fire all the time.
    if (!is_allied())
        attack();

    /// Rapid attacks are bound by is_attack() & m_attack_countdown bound to
    /// delta time - to respect game logic.
    // only proceed if is_attacking & attack countdown = 0
    if (m_is_attacking && m_attack_countdown <= sf::Time::Zero) {
        // queue attack in command queue - commands to be exec in order recieved
        commands.push(m_attack_command);
        // attack rate + 1, to divide 1 / 1 = remainder, attack rate in seconds
        m_attack_countdown += sf::seconds(1.f / (m_attack_rate + 1.f));
        // attack has been done! no longer attacking...
        m_is_attacking = false;
    }
    // if attack countdown > 0, decrement by delta time until = 0 -> to follow
    // game logic!
    else if (m_attack_countdown > sf::Time::Zero) {
        m_attack_countdown -= delta_time;
    }
    /// @todo Different styles of attack...
}

void Creature::create_projectile(SceneNode& node, Projectile::Type type,
        float x_offset, float y_offset, const TextureHolder& textures) const
{
    /// Smart pointer to projectile initialized on the heap.
    std::unique_ptr<Projectile> projectile(new Projectile(type, textures));
    // to create outside sprite -> offset is (x, y) offset * sprite (x, y)
    sf::Vector2f offset(x_offset * m_sprite.getGlobalBounds().width,
            y_offset * m_sprite.getGlobalBounds().height);
    // get velocity from max speed of projectile
    sf::Vector2f velocity(projectile->get_max_speed(),
            projectile->get_max_speed());
    // enemy projectiles go down, allied projectiles go up
    float sign = is_allied() ? -1.f : 1.f;

    // pos = pos + (offset from sprite * 1 || -1 (up or down enemy/friend))
    /// Uses setPosition(), a SFML member fn.
    projectile->setPosition(get_world_position() + (offset * sign));
    /// Uses set_velocity(), a user-defined member fn.
    projectile->set_velocity(velocity * sign); // up or down? enemy/friendly

    /// Attach the projectile as a child scene node of the scene graph.
    node.attach_child(std::move(projectile));
}

/**
 * Check if Creature is allied or not (Player or Ally).
 * @return True if allied, false if not.
 */
bool Creature::is_allied() const
{
    return m_type == Player;
}

/**
 * Get max speed of Creature.
 * @return Max speed of Creature::Type from data_tables.cpp.
 */
float Creature::get_max_speed() const
{
    return TABLE[m_type].speed;
}

/**
 * Sets m_is_attacking to true (to attack).
 * @note Only Creature(s) with an attack_interval that isn't zero are able to
 * attack.
 */
void Creature::attack()
{
    // guard to make sure attack_interval != 0
    if (TABLE[m_type].attack_interval != sf::Time::Zero)
        m_is_attacking = true;
}

void Creature::create_pickup(SceneNode& node, const TextureHolder& textures)
    const
{
    /// Get random pickup type, using random_int() and type count of pickups.
    auto type = static_cast<Pickup::Type>(random_int(Pickup::TypeCount));

    /// Create unique_ptr to pickup on the heap.
    std::unique_ptr<Pickup> pickup(new Pickup(type, textures));

    pickup->setPosition(get_world_position());
    pickup->set_velocity(0.f, 1.f);

    /// Attach pickup as child scene node on the scene graph.
    node.attach_child(std::move(pickup));
}
