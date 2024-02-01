#include "data_tables.h"
#include "creature.h"
#include "projectile.h"
#include "pickup.h"

// for std::bind() placeholders _1, _2, & so on...
using namespace std::placeholders;

std::vector<CreatureData> initialize_creature_data()
{
    // init vector to amt of creatures (typecount enum holds creature count)
    std::vector<CreatureData> data(Creature::TypeCount);

    // PLAYER DATA
    data[Creature::Player].hitpoints = 100.f;
    data[Creature::Player].speed = 75.f;
    data[Creature::Player].texture = Textures::Player;
    data[Creature::Player].attack_interval = sf::seconds(1);

    // BUNNY DATA
    data[Creature::Bunny].hitpoints = 5.f;
    data[Creature::Bunny].speed = 100.f;
    data[Creature::Bunny].texture = Textures::Bunny;
    // bunny pathing
    // first, move 150 px to the right (90 degree angle)
    data[Creature::Bunny].directions.push_back(Direction(90.f, 150.f));
    // move 300 px to the left (-90 degree angle)
    data[Creature::Bunny].directions.push_back(Direction(-90.f, 300.f));
    // move 150 px back to the right (90 degree angle) - back to origin -> reset
    data[Creature::Bunny].directions.push_back(Direction(90.f, 150.f));
    // bunny attack rate - every 2.5 sec
    data[Creature::Bunny].attack_interval = sf::seconds(2.5);

    // BEAR DATA
    data[Creature::Bear].hitpoints = 25.f;
    data[Creature::Bear].speed = 50.f;
    data[Creature::Bear].texture = Textures::Bear;
    // bear pathing
    // first, move 150 px to the right (90 degree angle)
    data[Creature::Bunny].directions.push_back(Direction(90.f, 150.f));
    // move 300 px to the left (-90 degree angle)
    data[Creature::Bunny].directions.push_back(Direction(-90.f, 300.f));
    // move 150 px back to the right (90 degree angle) - back to origin -> reset
    data[Creature::Bunny].directions.push_back(Direction(90.f, 150.f));
    // bear attack rate - every 4 sec
    data[Creature::Bunny].attack_interval = sf::seconds(4);

    return data;
}

std::vector<ProjectileData> initialize_projectile_data()
{
    std::vector<ProjectileData> data(Projectile::TypeCount);

    /** @brief Projectile::PlayerFire does 5.f damage, has 200.f speed, and
     * texture is Textures::FireProjectile */
    data[Projectile::PlayerFire].damage = 5.f;
    data[Projectile::PlayerFire].speed = 200.f;
    data[Projectile::PlayerFire].texture = Textures::FireProjectile;

    /** @brief Projectile::EnemyFire does 5.f damage, has 200.f speed, and
     * texture is Textures::FireProjectile */
    data[Projectile::EnemyFire].damage = 5.f;
    data[Projectile::EnemyFire].speed = 200.f;
    data[Projectile::EnemyFire].texture = Textures::FireProjectile;

    return data;
}

std::vector<PickupData> initialize_pickup_data()
{
    std::vector<PickupData> data(Pickup::TypeCount);

    data[Pickup::HealthRefill].texture = Textures::HealthRefill;
    /// HealthRefill refills 15.f HP.
    data[Pickup::HealthRefill].action = [] (Creature& c) { c.heal(15.f); };

    /// @todo Implement other pickups...
    /* data[Pickup::AttackRate].texture = Textures::AttackRate;
    data[Pickup::AttackRate].action = std::bind(&Creature::increase_attack_rate, _1);

    data[Pickup::Arrows].texture = Textures::Arrows;
    // binds std::placeholder::_1 to first param of Creature::collect_ammunition,
    // and amt 3 to second param
    data[Pickup::Arrows].action = std::bind(&Creature::collect_ammunition, _1, 3); */

    return data;
}
