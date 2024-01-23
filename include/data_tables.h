#pragma once

#include "entity.h"
#include "r_ids.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>
#include <functional>

// forward definition to use creature class
class Creature;

struct Direction {
    Direction(float angle, float distance) : angle(angle), distance(distance) {}
    float angle;
    float distance;
};

struct CreatureData {
    float hitpoints;
    float speed;
    Textures::ID texture;
    sf::Time attack_interval;
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
