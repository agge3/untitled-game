#include "scene_node.h"
#include "command.h"
#include "utility.h"
#include "command_queue.h"

/// RenderTarget, RectangleShape, and Color only needed locally for the
/// implementation of get_bounding_rect().
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>

/**
 * @note Collision between nodes on the scene graph are checked by (1) checking
 * the entire scene graph for collisions, (2) filling a set with collision pairs,
 * and (3) iterating through the set to differentiate between the collision's
 * categories
 */
SceneNode::SceneNode(Category::Type category) :
    m_children(), m_parent(nullptr), m_default_category(category) {}
// https://stackoverflow.com/questions/45583473/include-errors-detected-in-vscode

void SceneNode::attach_child(Ptr child) {
    child->m_parent = this;
    m_children.push_back(std::move(child));
}

// attach child node via setting parent pointer to curr node
SceneNode::Ptr SceneNode::detach_child(const SceneNode& node)
{
    // lambda returns true if node points to wanted address
    auto found = std::find_if(m_children.begin(), m_children.end(),
            [&] (Ptr& p) { return p.get() == &node; });

    // check for validity
    assert(found != m_children.end());

    // move found node out of container and into result
    // erase node's parent pointer from container and -> nullptr
    Ptr result = std::move(*found);
    result->m_parent = nullptr;
    m_children.erase(found);
    return result;
}

/// Public update interface for private update fn.
void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
    update_current(dt, commands);
    update_children(dt, commands);
}

void SceneNode::update_current(sf::Time, CommandQueue&)
{
    // do nothing by default
}

void SceneNode::update_children(sf::Time dt, CommandQueue& commands)
{
    for (Ptr& child : m_children)
        child->update(dt, commands);
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // combine the parent's absolute transformation with the current node's
    // relative one
    states.transform *= getTransform();
    // result is the absolute transform of the current node ->
    // stores where in the world scene node is placed

    // draw derived obj with states calling draw_current, forward both parameters
    draw_current(target, states);
    draw_children(target, states);
}

void SceneNode::draw_current(sf::RenderTarget&, sf::RenderStates) const
{
    // do nothing by default
}

void SceneNode::draw_children(sf::RenderTarget& target, sf::RenderStates states)
    const
{
    for (const Ptr& child : m_children)
        child->draw(target, states);
}

// absolute transformation functions ->
sf::Transform SceneNode::get_world_transform() const
{
    sf::Transform transform = sf::Transform::Identity;
    // loop to ittr & apply transforms?
    // TODO: rewrite/figure out more
    for (const SceneNode* node = this; node != nullptr; node = node->m_parent)
            transform = node->getTransform() * transform;
    return transform;
}

sf::Vector2f SceneNode::get_world_position() const
{
    return get_world_transform() * sf::Vector2f();
}

/**
 * @return Returns category of scene node.
 * @note Default category is Category::SceneGroundLayer.
 */
unsigned int SceneNode::get_category() const
{
    return Category::SceneGroundLayer;
}

void SceneNode::on_command(const Command& command, sf::Time dt)
{
    // command current node, if category matches
    if (command.category & get_category())
        command.action(*this, dt);
    // command children
    for (Ptr& child : m_children)
        child->on_command(command, dt);
}

/*
 * Draws the bounding rectangle used for collision detection around sprites.
 * @note Used for debugging, flag used to enable is in Debug class.
 */
void SceneNode::draw_bounding_rect(sf::RenderTarget& target,
        sf::RenderStates states) const
{
    sf::FloatRect rect = get_bounding_rect();

    sf::RectangleShape shape;
    shape.setPosition(sf::Vector2(rect.left, rect.top));
    shape.setSize(sf::Vector2f(rect.width, rect.height));
    /// Fill color can be made a solid color to have bounding rectangles be
    /// display as solid color rectangles.
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::Green);
    shape.setOutlineThickness(1.f);

    target.draw(shape);
}

/**
 * Checks the entire scene graph against one node for collisions.
 * @remark O(n^2) complexity, can be optimized.
 * @todo Implement by placing enemies in a grid and check collisions with
 * adjacent grid cells - will improve optimization.
 */
void SceneNode::check_node_collision(SceneNode& node,
        std::set<Pair>& collision_pairs)
{
    // caller != param, collision between caller & param, caller is not
    // destroyed, & param is not destroyed
    if (this != &node && collision(*this, node)
           && !is_destroyed() && !node.is_destroyed()) {
        /// collision_pairs is a map containing nodes with collision - insert
        /// into map. std::minmax() orders the map so that pairs are in an
        /// expected order, and there's no duplicate pairs.
        collision_pairs.insert(std::minmax(this, & node));
    }
    /// Recursively calls itself to iterate through all collision pairs for the
    /// calling node and its children.
    for (Ptr& child : m_children)
        child->check_node_collision(node, collision_pairs);
}

/**
 * Checks the whole scene graph against all nodes for collision.
 * @note Uses check_node_collision() and iterates over entire scene graph.
 */
void SceneNode::check_scene_collision(SceneNode& scene_graph,
        std::set<Pair>& collision_pairs)
{
    check_node_collision(scene_graph, collision_pairs);
    for (Ptr& child : scene_graph.m_children)
        check_scene_collision(*child, collision_pairs);
}

/**
 * Mark node for removal, to be handled appropriately.
 * @return Returns true if node is marked to be removed.
 */
bool SceneNode::is_marked_for_removal() const
{
    /// Uses is_destroyed() to determine if marked for removal.
    return is_destroyed();
}


/// Virtual fn for derived class(es) to implement and handle.
bool SceneNode::is_destroyed() const
{
    /// By default, scene node is not to be removed.
    return false;
}

/**
 * Removal routine.
 * @if is_marked_for_removal().
 * @return No return value, but creates a fn object for World::update to use
 * to remove nodes that request their removal.
 * @remarks Removal routine allows special routines to be ran before node is
 * formally removed, such as animations, particle effects, etc.
 */
void SceneNode::removal()
{
    /// Rearranges the children container, so that all active nodes are at the
    /// beginning and all nodes to be removed are at the end.
    auto removal_begin = std::remove_if(m_children.begin(), m_children.end(),
            std::mem_fn(&SceneNode::is_marked_for_removal));
    /// Erases the SceneNode::Ptr objects to be removed.
    m_children.erase(removal_begin, m_children.end());
    /// Recursive fn call to create a fn object for each node marked for removal
    /// - fn object to be used in World::update.
    std::for_each(m_children.begin(), m_children.end(),
            std::mem_fn(&SceneNode::removal));
}

/**
 * @return Returns the bounding rectangle of current scene node.
 */
sf::FloatRect SceneNode::get_bounding_rect() const
{
    return sf::FloatRect();
}

/*
 * Checks collisions between bounding rectangles in the scene graph.
 * @note Not implemented in Entity class because collisions occur in the scene
 * graph, not between entities - avoids downcasting.
 * @remark Global function to allow other classes to access, checking collisions
 * is important to a lot of systems!
 */
bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
    /// Uses sf::FloatRect::Intersects to determine if bounding rectangle
    /// intersects with another bounding rectangle -> if so, collision has
    /// occured.
    return lhs.get_bounding_rect().intersects(rhs.get_bounding_rect());
}

/**
 * Get distance between two bounding rectangles in the scene graph.
 * @return Returns float distance.
 * @remark Global function to allow other classes to access, checking distance
 * between scene nodes is important to a lot of systems!
 */
float distance(const SceneNode& lhs, const SceneNode& rhs)
{
    /// Uses length fn for Utility class to calculate distance
    return length(lhs.get_world_position() - rhs.get_world_position());
}
