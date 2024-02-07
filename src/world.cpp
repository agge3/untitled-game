#include <world.h>

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>

World::World(sf::RenderWindow& window, FontHolder& fonts) :
    // initialize all parts of the world correctly
    // window first ->
    m_window(window),

    // systems second ->
    m_textures(),
    m_fonts(fonts),
    m_scene_graph(),
    m_scene_layers(),

    // world third ->
    // set the size of the world
    m_world_bounds(0.f, 0.f, 5000.f, 5000.f),
    // set player view to be zoomed in, keeping aspect ratio
    m_world_view(sf::FloatRect(0.f, 0.f, 480.f, 270.f)),
    m_scroll_speed(0.f),

    // player fourth ->
    m_player_creature(nullptr),
    // spawn player in the center of the world
    m_player_spawn_point(m_world_bounds.width / 2.f,
            m_world_bounds.height / 2.f),

    // npcs fifth ->
    m_npc_spawn_points(),
    m_active_npcs()
{
        load_textures();
        build_scene();

        /// Prepare the view - set center to player spawn point.
        m_world_view.setCenter(m_player_spawn_point);
}

void World::update(sf::Time delta_time)
{
    m_world_view.move(0.f, m_scroll_speed * delta_time.asSeconds());
    m_player_creature->set_velocity(0.f, 0.f);

    /** @brief Setup commands to destroy entities if outside world chunk.
     * @warning NOT USED. */
    //destroy_entities_outside_chunk();

    /** @brief Forward commands to the scene graph and adapt player velocity
     * correctly. */
    while (!m_command_queue.is_empty())
        m_scene_graph.on_command(m_command_queue.pop(), delta_time);
    adapt_player_velocity();

    /// Constantly update collision detection and response (WARNING: May destroy
    /// entities).
    handle_collisions();

    /// Remove all destroyed entities and create new ones.
    m_scene_graph.removal();
    spawn_npcs();

    /// Regular game update step, adapt player position (correct even though
    /// outside view, because adapt_player_position() handles appropriately).
    m_scene_graph.update(delta_time, m_command_queue);
    adapt_player_position();
}

void World::draw()
{
    m_window.setView(m_world_view);
    m_window.draw(m_scene_graph);
}

/**
 * Get command queue from outside the world.
 * @return Return the command queue.
 */
CommandQueue& World::get_command_queue()
{
    return m_command_queue;
}

void World::load_textures()
{
    m_textures.load(Textures::Grass, "textures/world/grass1.png");

    m_textures.load(Textures::Player, "textures/player/player.png");
    m_textures.load(Textures::FireProjectile, "textures/player/player.png");

    m_textures.load(Textures::Bunny, "textures/player/player.png");
    m_textures.load(Textures::Bear, "textures/player/player.png");

    m_textures.load(Textures::HealthRefill, "textures/player/player.png");
}

void World::build_scene()
{
    /// Initialize all the different scene layers.
    for(std::size_t i = 0; i < LayerCount; ++i) {
        SceneNode::Ptr layer(new SceneNode());
        m_scene_layers[i] = layer.get();

        m_scene_graph.attach_child(std::move(layer));
    }

    // Prepare tiled background.
    sf::Texture& texture = m_textures.get(Textures::Grass);
    sf::IntRect texture_rect(m_world_bounds);
    texture.setRepeated(true);

    // Add background sprite to the scene.
    std::unique_ptr<SpriteNode> background_sprite(new SpriteNode(
                texture, texture_rect));
    background_sprite->setPosition(m_world_bounds.left, m_world_bounds.top);
    m_scene_layers[Background]->attach_child(std::move(background_sprite));

    // Add player character to the scene.
    std::unique_ptr<Creature> player(new Creature(
                Creature::Player, m_textures, m_fonts));
    m_player_creature = player.get();
    m_player_creature->setPosition(m_player_spawn_point);
    m_scene_layers[Foreground]->attach_child(std::move(player));

    /// Add NPCs to the scene.
    add_npcs();
}

void World::adapt_player_position()
{
    /// Initialize view bounds to world view.
  	sf::FloatRect view_bounds(m_world_view.getCenter()
            - m_world_view.getSize() / 2.f, m_world_view.getSize());
    /// Initialize distance from "border" (before view pan).
    // what distance from border before panning view? 16.f = 16px
	constexpr float border_distance = 16.f;

    /// Initialize position to player position.
	sf::Vector2f position = m_player_creature->getPosition();

	// pos.x = pos.x || (0x + border_dist, y), if pos.x <= pan neg to the left
    // on the x-axis
    if (position.x = std::max(position.x, view_bounds.left + border_distance),
            position.x <= view_bounds.left + border_distance)
        // pan half distance of view bounds (world view)
        m_world_view.move(-(view_bounds.width / 2), 0.f);
    // (WIDTHx - border_dist, y), if pos.x >= pan pos to the right on the x-asis
    if (position.x = std::min(position.x,
            view_bounds.left + view_bounds.width - border_distance),
            position.x >= view_bounds.left + view_bounds.width - border_distance)
        m_world_view.move(view_bounds.width / 2, 0.f);
    // same as x-axis, for y-axis
    if (position.y = std::max(position.y, view_bounds.top + border_distance),
            position.y <= view_bounds.top + border_distance)
        m_world_view.move(0.f, -(view_bounds.height / 2));
	if (position.y = std::min(position.y,
            view_bounds.top + view_bounds.height - border_distance),
            position.y >= view_bounds.top + view_bounds.height - border_distance)
        m_world_view.move(0.f, view_bounds.height / 2);

    /// Set player position to current position.
	m_player_creature->setPosition(position);

    // uncomment to print current player pos
    std::cout << "Player position: (" << position.x << ", " << position.y << ")\n";
}

void World::adapt_player_velocity()
{
    sf::Vector2f velocity = m_player_creature->get_velocity();

    // if moving diagonally, reduce velocity (to always have same velocity)
    if (velocity.x != 0.f && velocity.y != 0.f)
        m_player_creature->set_velocity(velocity / std::sqrt(2.f));

    // add scrolling velocity
    m_player_creature->accelerate(0.f, m_scroll_speed);
}

/**
 * Spawns NPCs from m_npc_spawn_points.
 * @see add_npcs().
 * @note Assumes m_npc_spawn_points is sorted in ascending order. Iterating
 * from lowest to highest coordinates is more likely to be the common case.
 */
void World::spawn_npcs()
{
    /** @todo Decide on whether to implement world chunks or not.
    // if there's avail spawn points...
    if (!m_npc_spawn_points.empty()) {
        // iter through spawn points until at current world chunk (...lowest
        // coord are more likely to be the common case)
        for (auto iter = m_npc_spawn_points.begin();
                iter != m_npc_spawn_points.end(); ++iter) {
            // if spawn points are within current world chunk...
            MATH: spawn.x > chunk.left
            && spawn.x < (chunk.left + chunk.size.x)
            && spawn.y < chunk.top
            && spawn.y > (chunk.top - chunk.size.y)
            if (iter->x > get_chunk_bounds().left
                    && iter->x < (get_chunk_bounds().left
                        + get_chunk_bounds().getSize().x)
                    && iter->y < get_chunk_bounds().top
                    && iter->y > (get_chunk_bounds().top
                        - get_chunk_bounds().getSize().y)) {
                // ... spawn & del from vec...
                // init SpawnPoint after check to not create unneccesary structs
                SpawnPoint spawn = *iter;
                // create smart ptr to npc spawn on heap
                std::unique_ptr<Creature> npc(
                        new Creature(spawn.type, m_textures, m_fonts));
                std::cout << spawn.type << " spawned in the world!"
                    << std::endl;
                // set enemy pos to spawn pos
                npc->setPosition(spawn.x, spawn.y);
                // bind to foreground layer
                m_scene_layers[Foreground]->attach_child(std::move(npc));
                // remove spawn point from vec & keep iter for valid spawns
                iter = m_npc_spawn_points.erase(iter);
            }
            // TODO: break out of loop when spawn point > chunk to not do
            // unneccesary iter
        }
    }
    */
    // if npc spawn points vector is not empty...
    if (!m_npc_spawn_points.empty()) {
        // iter through each spawn point and spawn
        for (auto iter = m_npc_spawn_points.rbegin();
                iter != m_npc_spawn_points.rend(); ++iter) {
            // init SpawnPoint AFTER check to not create unneccesary structs
            SpawnPoint spawn = *iter;
            // create smart ptr to spawn npc on heap
            std::unique_ptr<Creature> npc(
                    new Creature(spawn.type, m_textures, m_fonts));
            // set enemy pos to spawn pos
            npc->setPosition(spawn.vec2.x, spawn.vec2.y);
            // print success and pos for confirmation
            std::cout << spawn.type << " spawned in the world!" << " ("
                << std::setprecision(0) << spawn.vec2.x << ", " << spawn.vec2.y
                << ")\n";

            // bind to foreground layer
            m_scene_layers[Foreground]->attach_child(std::move(npc));
            // riter, so pop_back end and "increment" backwards (moving to begin
            // until vector is empty and all spawn points have been spawned)
            m_npc_spawn_points.pop_back();
        }
    }
}

/**
 * Adds ONE NPC at a time to m_npc_spawn_points.
 * @param Creature::Type type
 * The type of the Creature to be added.
 * @param const sf::Vector2& vec2_rel
 * The relative position (to the Player) of the Creature to be added.
 * @todo Should vec2_rel be const or not?
 * @attention Used by add_npcs() to add ALL NPCs. Is an abstraction, to be
 * reused.
 * @see add_npcs()
 */
void World::add_npc(Creature::Type type, sf::Vector2f& vec2_rel)
{
    // spawn with enemy type, spawn pos + player pos -> to spawn rel to player
    sf::Vector2f rel{m_player_spawn_point.x + vec2_rel.x,
        m_player_spawn_point.y + vec2_rel.y};
    SpawnPoint spawn(type, rel);
    // after init spawn with enemy type and pos of spawn, push into spawn point
    // vec
    m_npc_spawn_points.push_back(spawn);
    std::cout << spawn.type << " added to NPC spawn points" << std::endl;
}

/**
 * Adds ALL NPCs to be spawned to m_npc_spawn_points.
 * @note Uses add_npc() abstraction to add NPCs to m_npc_spawn_points.
 * @see add_npc().
 * @note Sorts NPCs in descending order, based on y-axis.
 * @remark Does NOT SPAWN NPCs.
 * @see spawn_npcs().
 */
void World::add_npcs()
{
    sf::Vector2f bunny_spawn{20.f, 50.f};
    sf::Vector2f bear_spawn{100.f, 200.f};
    //add_npc(Creature::Bunny, bunny_spawn);
    //add_npc(Creature::Bear, bear_spawn);

    // sort enemy spawn points so they can be iter through in an expected way
    std::sort(m_npc_spawn_points.begin(), m_npc_spawn_points.end(), []
            (SpawnPoint lhs, SpawnPoint rhs) {
        // sorting algorithm used - descending sort, based on y-axis
        return (lhs.vec2.y < rhs.vec2.y);
    });
}

/**
 * Destroys Entity(ies) outside current world chunk, to reduce resource strain.
 * @warning NOT USED.
 * @todo Decide whether to implement chunk system or not?
 */
void World::destroy_entities_outside_chunk()
{
    /*
    Command command;
    /// Entities to be destroyed: Projectile and EnemyNpc.
    command.category = Category::Projectile | Category::EnemyNpc;
    command.action = derived_action<Entity>([this] (Entity& e, sf::Time) {
            /// If entity bounding rectangle leaves chunk bounds, then destroy.
            if (!get_chunk_bounds().intersects(e.get_bounding_rect()))
                e.destroy();
    });
    /// Push destroy() command into command queue.
    m_command_queue.push(command);
    */
}


/** Helper function that determines if the colliding scene nodes match certain
 * expected categories.
 * @return Returns true if the colliders match the expected categories, false
 * if not.
 * @remark There are four types of collisions that are checked to match certain
 * categories - Player/Pickup, Player/EnemyNpc, Player/EnemyProjectile,
 * PlayerProjectile/EnemyNpc.
 */
bool World::matches_categories(SceneNode::Pair& colliders,
        Category::Type type1, Category::Type type2) const
{
    /// Colliders are stored in a pair, first and second are colliders.
    unsigned int category1 = colliders.first->get_category();
    unsigned int category2 = colliders.second->get_category();
    // can perform bitwise comparison because unsigned ints
    if (type1 & category1 && type2 & category2)
        return true;
    else if (type1 & category2 && type2 & category1) {
        // still a match! just not in expected order
        /// If not in expected order, swap pair so that it is in expected order,
        /// also return true because the colliders match the expected types.
        std::swap(colliders.first, colliders.second);
        return true;
    } else
        return false;
}

/**
 * @return Returns a sf::FloatRect that is the bounds of the view.
 */
sf::FloatRect World::get_view_bounds() const
{
    // view bounds: x = (world center - world size) / 2, y = world size
    return sf::FloatRect(m_world_view.getCenter() - m_world_view.getSize() / 2.f,
            m_world_view.getSize());
}

/**
 * @return Returns a sf::FloatRect that is the bounds of the current world chunk.
 * @note Chunks are a pixel offset from current view bounds (x, y).
 */
sf::FloatRect World::get_chunk_bounds() const
{
    /// Based on current view bounds.
    sf::FloatRect bounds = get_view_bounds();
    /// Chunks are 1000 pixel offset from current view bounds (x, y).
    bounds.top -+ 1000.f;
    bounds.height += 1000.f;
    return bounds;
}

/** Uses matches_categories() to decide how to handle each collider pair as
 * desired.
 */
void World::handle_collisions()
{
    /// Initialize collision_pairs set for scene graph to use.
    std::set<SceneNode::Pair> collision_pairs;
    m_scene_graph.check_scene_collision(m_scene_graph, collision_pairs);
    for (SceneNode::Pair pair : collision_pairs) {
        /// For Player/Pickup, apply the pickup to the player and destroy the
        /// pickup.
        if (matches_categories(pair, Category::Player, Category::PlayerPickup)) {
            // static cast the pair's type to the expected type to make sure
            // (safe because the pair's type is expected to match), and create
            // local variables storing each - to work with.
            auto& player = static_cast<Creature&>(*pair.first);
            auto& pickup = static_cast<Pickup&>(*pair.second);
            pickup.apply(player);
            pickup.destroy();
        } else if (matches_categories(pair, Category::Player,
                    Category::EnemyNpc)) {
            /// For Player/EnemyNpc, damage the player and destroy the enemy.
            auto& player = static_cast<Creature&>(*pair.first);
            auto& enemy = static_cast<Creature&>(*pair.second);
            player.damage(enemy.get_hitpoints());
            enemy.destroy();
        } else if (matches_categories(pair, Category::Player,
                    Category::EnemyProjectile)
                    || matches_categories(pair, Category::PlayerProjectile,
                    Category::EnemyNpc)) {
            /// For Player/EnemyProjectile and PlayerProjectile/EnemyNpc
            /// (handled the same because projectiles are to be handled the same,
            /// regardless of recipient), damage the recipient and destroy the
            /// projectile.
            auto& creature = static_cast<Creature&>(*pair.first);
            auto& projectile = static_cast<Projectile&>(*pair.second);
            creature.damage(projectile.get_damage());
            projectile.destroy();
        }
    }
}
