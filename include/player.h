#pragma once

//#define SFML_STATIC

#include "command.h"

#include <SFML/Window/Event.hpp>

#include <map>

/** @brief Forward definition of CommandQueue to be used in implementation. */
class CommandQueue;

/**
 * @class Player
 * Controls input handling of Player Creature.
 */
class Player {
public:
    /**
     * @enum Action
     * Player actions - to allow keybinding.
     */
    enum Action : unsigned int {
        None = 0,
        MoveUp = 1 << 0,
        MoveDown = 1 << 1,
        MoveLeft = 1 << 2,
        MoveRight = 1 << 3,
        MeleeAttack = 1 << 4,
        RangedAttack = 1 << 5,
        MagicAttack = 1 << 6,
        SpecialAttack = 1 << 7,
        Inventory = 1 << 8,
        Map = 1 << 9,
    };

    /**
     * @enum LevelStatus
     * Current level status.
     */
    enum LevelStatus : unsigned int {
        /** @todo Migrate to enum class. */
        //None = 0,
        InProgress = 1 << 0,
        Success = 1 << 1,
        Failure = 1 << 2,
    };

    Player();
    // for one-time key presses
    void handle_event(const sf::Event& event, CommandQueue& commands);
    // for real-time input
    void handle_realtime_input(CommandQueue& commands);
    // fn to bind keys and get assigned keys
    void assign_key(Action action, sf::Keyboard::Key key);
    sf::Keyboard::Key get_assigned_key(Action action) const;
    void set_level_status(LevelStatus status);
    LevelStatus get_level_status() const;
    /** @todo Returns nullptr until implemented. */
    char* print_assigned_key(Action action) const;
private:
    void initialize_actions();
    static bool is_realtime_action(Action action);

    /**
     * @var std::map<sf::Keyboard::Key, Action> m_keybinding
     * Key to bind to action.
     * @note Store the keys and actions in map together instead of as seperate
     * variables.
     */
    std::map<sf::Keyboard::Key, Action> m_keybinding;

    /**
     * @var std::map<Action, Command> m_actionbinding
     * Action to bind to command.
     * @note Store the actions and commands in a map together instead of as
     * seperate variables.
     */
    std::map<Action, Command> m_actionbinding;

    /**
     * @var LevelStatus m_current_level_status
     * Holds current level status of Player.
     */
    LevelStatus m_current_level_status;
};
