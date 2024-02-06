/**
 * @project untitled game
 * @copyright See LICENSE in the top level directory
 * @file data_tables.h
 * @developers agge3
 */

#pragma once

#include "entity.h"
#include "r_ids.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>
#include <functional>

// forward definition to use creature class
class Creature;

/**
 * @struct Direction
 * Struct to store the direction of the Creature, includes angle and distance.
 */
struct Direction {
    Direction(float angle, float distance) : angle(angle), distance(distance) {}
    float angle; /**< float angle of the Creature. */
    float distance; /**< float distance of the Creature. */
};

/**
 * @struct CreatureData
 * Struct to store data of the Creature, hitpoints, speed, etc.
 */
struct CreatureData {
    float hitpoints; /**< float hitpoints for Creature hitpoints. */
    float speed; /**< float speed for Creature speed. */
    Textures::ID texture; /**< Textures::ID enum to texture. */
    sf::Time attack_interval; /**< Specified attack_interval in sf::Time. */
    /** std::vector of struct Direction, storing all directions. */
    std::vector<Direction> directions;
};

struct ProjectileData {
    float damage;
    float speed;
    Textures::ID texture;
};

struct PickupData {
    std::function<void(Creature&)> action;
    Textures::ID texture;
};

std::vector<CreatureData> initialize_creature_data();
std::vector<ProjectileData> initialize_projectile_data();
std::vector<PickupData> initialize_pickup_data();
