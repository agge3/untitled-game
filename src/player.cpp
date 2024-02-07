#include "player.h"
#include "command_queue.h"
#include "creature.h"

#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace std::placeholders;

struct PlayerMover {
    PlayerMover(float vx, float vy) : velocity(vx, vy) {}
    void operator() (Creature& player, sf::Time) const
    {
        player.accelerate(velocity * player.get_max_speed());
        // uncomment to print current player velocity
        std::cout << "Player velocity: (" << velocity.x * player.get_max_speed()
            << ", " << velocity.y * player.get_max_speed() << ")\n";
    }
    sf::Vector2f velocity;
};

/**
 * @note Default LevelStatus of Player is InProgress. Initialized in default
 * constructor.
 */
Player::Player() : m_current_level_status(InProgress)
{
    /// Try to set initial keybindings.
    try {
        m_keybinding[sf::Keyboard::Up] = MoveUp;
        std::cout << "Player keybind: Move up = Up arrow\n";
        m_keybinding[sf::Keyboard::Down] = MoveDown;
        std::cout << "Player keybind: Move down = Down arrow\n";
        m_keybinding[sf::Keyboard::Left] = MoveLeft;
        std::cout << "Player keybind: Move left = Left arrow\n";
        m_keybinding[sf::Keyboard::Right] = MoveRight;
        std::cout << "Player keybind: Move right = Right arrow\n";

        // attack actions...
        m_keybinding[sf::Keyboard::Space] = MagicAttack;
        // set inital actionbindings
        initialize_actions();
    } catch (std::exception& e) {
        /// Catch exception and print error message.
        std::cerr << "\n EXCEPTION: " << e.what() <<
            ". Failed to initialize default player keybinds" << std::endl;
    }
    /** @brief All categories assigned to Player Creature. */
    for (auto& pair : m_actionbinding) {
        // uncomment to print successful action category assigned to player
        // category
        std::cout << "Action category assigned to player category\n";
        pair.second.category = Category::Player;
    }
}

/** To check if real-time action and not event (i.e., player wants to assign key). */
void Player::handle_event(const sf::Event& event, CommandQueue& commands)
{
    if (event.type == sf::Event::KeyPressed) {
        // check if pressed key appears in keybinding, trigger command if so
        auto found = m_keybinding.find(event.key.code);
        if (found != m_keybinding.end() && !is_realtime_action(found->second))
            commands.push(m_actionbinding[found->second]);
    }
}

void Player::handle_realtime_input(CommandQueue& commands)
{
    /** @brief Traverses all assigned keys and checks if they are pressed. */
    for (auto pair : m_keybinding) {
        /** @brief If key is pressed, lookup action and trigger correspoding
         * command. */
        if (sf::Keyboard::isKeyPressed(pair.first)
                && is_realtime_action(pair.second)) {
            // print detection of realtime input
            std::cout << "Realtime input detected!\n";
            commands.push(m_actionbinding[pair.second]);
        }
    }
}

void Player::assign_key(Action action, sf::Keyboard::Key key)
{
    // remove all keys that already map to action
    /** @todo Rewrite for loop. */
    for (auto iter = m_keybinding.begin(); iter != m_keybinding.end(); ) {
        if(iter->second == action)
            m_keybinding.erase(iter++);
        else
            ++iter;
    }
    // insert new binding
    m_keybinding[key] = action;
}

sf::Keyboard::Key Player::get_assigned_key(Action action) const
{
    for (auto pair : m_keybinding) {
        if(pair.second == action)
            return pair.first;
    }
    return sf::Keyboard::Unknown;
}

char* Player::print_assigned_key(Action action) const
{
    sf::Keyboard::Key key = get_assigned_key(action);
    // convert sfml key into string...
    return nullptr;
}

/**
 * @attention Player speed is obtained from data_tables.cpp. Correct direction
 * is achieved by multiplying by positive or negative one on the correct axis.
 * @see PlayerMover, Creature::get_max_speed(), data_tables.cpp
 * @note Will attack every frame.
 * @see Creature::check_projectile_launch() to sync with delta time.
 */
void Player::initialize_actions() {
    /** @brief Movement commands increment and decrement player speed. */
    // @note y-axis up/down pos/neg is inverse!
	m_actionbinding[MoveUp].action = derived_action<Creature>(
            PlayerMover(0.f, -1.f));
    std::cout << "Player action initialized: Move up\n";
	m_actionbinding[MoveDown].action = derived_action<Creature>(
            PlayerMover(0.f, +1.f));
    std::cout << "Player action initialized: Move down\n";
    m_actionbinding[MoveLeft].action = derived_action<Creature>(
            PlayerMover(-1.f, 0.f));
    std::cout << "Player action initialized: Move left\n";
	m_actionbinding[MoveRight].action = derived_action<Creature>(
            PlayerMover(+1.f, 0.f));
    std::cout << "Player action initialized: Move right\n";

    // attack actions...
    // std::bind binds the para "_1" to always be the para for &attack ...
    // -> Creature::attack(std::placeholders::_1);
    m_actionbinding[MagicAttack].action = derived_action<Creature>(
            std::bind(&Creature::attack, _1));
    // Creature::check_projectile_launch() ->
    // guards to properly attack based on delta time
}

/**
 * Checks if real-time action (immediate action), as opposed to queued action.
 * @return TRUE if real-time action, FALSE if not.
 * @note Default is FALSE. If an action is not explicitly defined as real-time,
 * then it is not to be handled as real-time.
 */
bool Player::is_realtime_action(Action action)
{
    // all actions to be handled real-time...
    switch (action) {
    case MoveUp:
    case MoveDown:
    case MoveLeft:
    case MoveRight:
    case MagicAttack:
        return true;
        break;
    // if not explicitly defined as real-time, return false -> don't handle
    // as real-time
    default:
        return false;
        break;
    }
}

/**
 * Use to set current LevelStatus of Player.
 */
void Player::set_level_status(LevelStatus status)
{
    m_current_level_status = status;
}

/**
 * Use to get current LevelStatus of Player.
 * @return m_current_level_status, enum of LevelStatus.
 */
Player::LevelStatus Player::get_level_status() const
{
    return m_current_level_status;
}
