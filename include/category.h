#pragma once

// entity/scene node category, used to dispatch commands
namespace Category {
    enum Type {
        // bit-wise category for highest efficiency & bit-wise OR to group
        None = 0,
        SceneGroundLayer = 1 << 0,
        Player = 1 << 1, // player
        FriendlyNpc = 1 << 2, // friendly to player
        NeutralNpc = 1 << 3, // neutral to both sides
        EnemyNpc = 1 << 4, // enemy to player
        PlayerProjectile = 1 << 5,
        FriendlyProjectile = 1 << 6,
        NeutralProjectile = 1 << 7,
        EnemyProjectile = 1 << 8,
        PlayerPickup = 1 << 9,
        FriendlyPickup = 1 << 10,
        NeutralPickup = 1 << 11,
        EnemyPickup = 1 << 12,
        // TODO: decide if there needs to be player, friendly, neutral, enemy
        // for every entity

        Creature = Player | FriendlyNpc | NeutralNpc | EnemyNpc,
        Projectile = PlayerProjectile | FriendlyProjectile | NeutralProjectile
            | EnemyProjectile,
        Pickup = PlayerPickup | FriendlyPickup | NeutralPickup | EnemyPickup,
    };
}
