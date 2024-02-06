#pragma once

//#define SFML_STATIC

#include "r_holders.h"
#include "r_ids.h"
#include "scene_node.h"
#include "sprite_node.h"
#include "creature.h"
#include "command_queue.h"
#include "command.h"
#include "pickup.h"
#include "projectile.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>
#include <vector>

/// Forward declarations to be used in implementation.
namespace sf {
	class RenderWindow;
}

/**
 World must include...
   * a ref to render window
   * the world's current view
   * a texture holder with all the features needed inside the world
   * the scene graph
   * some ptrs to access scene graph's layer nodes

   logical data...
   * the bounding rect of world, storing dimensions
   * the pos where player's plane appears in the beginning
   * the speed with which the world is scrolled
   * a pointer to the player's sprite
*/

class World : private sf::NonCopyable { // non copyable, one world
public:
    explicit World(sf::RenderWindow& window, FontHolder& fonts);

    void update(sf::Time dt);
    void draw();
    CommandQueue& get_command_queue();
private:
    /** @enum Layer
     * An enum for the world layers.
     */
    enum Layer {
        Background,
        Foreground,
        LayerCount
    };

    /**
     * @struct SpawnPoint
     * Spawn point struct, with Creature::Type and sf::Vector2f (x, y)
     * coordinates for spawn.
     */
    struct SpawnPoint {
        SpawnPoint(Creature::Type type, sf::Vector2f& vec2) :
            type(type), vec2(vec2) {}
        Creature::Type type;
        sf::Vector2f vec2;
    };

    void load_textures();
    void build_scene();
	void adapt_player_position();
	void adapt_player_velocity();
    void handle_collisions();
    void add_npcs();
    void add_npc(Creature::Type type, sf::Vector2f& vec2_rel);
    void spawn_npcs();
    void destroy_entities_outside_chunk();
    // void guide_projectiles();
    bool matches_categories(SceneNode::Pair& colliders, Category::Type type1,
            Category::Type type2) const;
    sf::FloatRect get_view_bounds() const;
    sf::FloatRect get_chunk_bounds() const;

    sf::RenderWindow& m_window;
    sf::View m_world_view;
    TextureHolder m_textures;
    /// FontHolder is reference and TextureHolder is not because of FontHolder&
    /// in default constructor.
    FontHolder& m_fonts;
    SceneNode m_scene_graph;
    /// For scene layers, use array of Ptr with the size LayerCount.
    std::array<SceneNode*, LayerCount> m_scene_layers;
	CommandQueue m_command_queue;
    sf::FloatRect m_world_bounds;
    sf::Vector2f m_player_spawn_point;
    float m_scroll_speed;
    Creature* m_player_creature;
    /// Holds all future spawn points.
    std::vector<SpawnPoint> m_npc_spawn_points;
    /// Holds Ptr to all active NPCs.
    std::vector<Creature*> m_active_npcs;
};
