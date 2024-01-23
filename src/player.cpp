#include "player.h"
#include "command_queue.h"
#include "creature.h"

#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdexcept>

struct CreatureMover {
    CreatureMover(float vx, float vy) : velocity(vx, vy) {}
    void operator() (Creature& creature, sf::Time) const
    {
        creature.accelerate(velocity);
    }
    sf::Vector2f velocity;
};

Player::Player()
{
    /// Try to set initial keybindings.
    try {
        m_keybinding[sf::Keyboard::Up] = MoveUp;
        m_keybinding[sf::Keyboard::Down] = MoveDown;
        m_keybinding[sf::Keyboard::Left] = MoveLeft;
        m_keybinding[sf::Keyboard::Right] = MoveRight;
        // attack actions...
        m_keybinding[sf::Keyboard::Space] = MagicAttack;
        // set inital actionbindings
        initialize_actions();
    } catch (std::exception& e) {
        /// Catch exception and print error message.
        std::cerr << "\n EXCEPTION: " << e.what() <<
            ". Failed to initialize default player keybinds" << std::endl;
    }
    // assign all categories to player's entity
    for (auto& pair : m_actionbinding)
        pair.second.category = Category::Player;
}

/// To check if real-time action and not event (i.e., player wants to assign key).
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
    // traverse all assigned keys and check if they are pressed
    for (auto pair : m_keybinding) {
        // if key is pressed, lookup action and trigger correspoding command
        if (sf::Keyboard::isKeyPressed(pair.first) && is_realtime_action(pair.second))
                commands.push(m_actionbinding[pair.second]);
    }
}

void Player::assign_key(Action action, sf::Keyboard::Key key)
{
    // remove all keys that already map to action
    // TODO: rewrite for loop
    for (auto itr = m_keybinding.begin(); itr != m_keybinding.end(); ) {
        if(itr->second == action)
            m_keybinding.erase(itr++);
        else
            ++itr;
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
 * @note Will atack every frame.
 * @see Creature::check_projectile_launch() to sync with delta time.
 */
void Player::initialize_actions() {
    float PLAYER_SPEED = 75.f;
    /// Movement commands increment and decrement player speed.
    m_actionbinding[MoveLeft].action = derived_action<Creature>(
            CreatureMover(-PLAYER_SPEED, 0.f));
	m_actionbinding[MoveRight].action = derived_action<Creature>(
            CreatureMover(+PLAYER_SPEED, 0.f));
	m_actionbinding[MoveUp].action = derived_action<Creature>(
            CreatureMover(0.f, -PLAYER_SPEED));
	m_actionbinding[MoveDown].action = derived_action<Creature>(
            CreatureMover(0.f, +PLAYER_SPEED));
    // attack actions...
    // std::bind binds the para "_1" to always be the para for &attack ...
    // -> Creature::attack(std::placeholders::_1);
    m_actionbinding[MagicAttack].action = derived_action<Creature>(
            std::bind(&Creature::attack, std::placeholders::_1));
    // Creature::check_projectile_launch() ->
    // guards to properly attack based on delta time
}

bool Player::is_realtime_action(Action action)
{
    // all actions to be handled realtime...
    switch (action) {
    case MoveUp:
        return true;
        break;
    case MoveDown:
        return true;
        break;
    case MoveLeft:
        return true;
        break;
    case MoveRight:
        return true;
        break;
    case MagicAttack:
        return true;
        break;
    // if not explicitly defined as realtime, return false -> don't handle realtime
    default:
        return false;
        break;
    }
}
