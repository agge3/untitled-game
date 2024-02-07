#pragma once

//#define SFML_STATIC

#include "category.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <memory>
#include <set>
#include <utility>

/// Forward declaration of RenderTarget - only used locally in get_bounding_rect().
namespace sf {
    class RenderTarget;
}

/** @brief Forward declaration of Command to be used in implementation. */
struct Command;
/** @brief Forward declaration of CommandQueue to be used in implementation. */
struct CommandQueue;

class SceneNode :
    public sf::Transformable, // store its curr pos, rotatation,
                              // and scale + provide interface
    public sf::Drawable, // scene nodes are drawable
    private sf::NonCopyable { // scene nodes cannot be copied
public:
    /**
     * @typedef std::unqiue_ptr<SceneNode> Ptr
     * Ptr is a unique pointer to a SceneNode.
     */
    typedef std::unique_ptr<SceneNode> Ptr;

    /**
     * @typedef std::pair<SceneNode*, SceneNode*> Pair
     * Pair is a pair containing a SceneNode Ptr and a SceneNode Ptr.
     */
    typedef std::pair<SceneNode*, SceneNode*> Pair;

    explicit SceneNode(Category::Type category = Category::None);

    void attach_child(Ptr child);
    Ptr detach_child(const SceneNode& node);
    // update scene
    void update(sf::Time delta_time, CommandQueue& commands);
    // absolute transformations
    sf::Transform get_world_transform() const;
    sf::Vector2f get_world_position() const;
    // virtual method that returns category of the game obj
    virtual unsigned int get_category() const;
    // non-virtual method, pass command to scene graph
    void on_command(const Command& command, sf::Time dt);
    void check_node_collision(SceneNode& node, std::set<Pair>& collision_pairs);
    void check_scene_collision(SceneNode& scene_graph,
            std::set<Pair>& collision_pairs);
    virtual bool is_marked_for_removal() const;
    virtual bool is_destroyed() const;
    void removal();
    sf::FloatRect get_bounding_rect() const;
private:
    // to be overwritten by derived classes
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    // only draw current object, to be overwritten by derived classes
    virtual void draw_current(sf::RenderTarget& target, sf::RenderStates states)
        const;
    void draw_children(sf::RenderTarget& target, sf::RenderStates states) const;
    // update parent
    virtual void update_current(sf::Time dt, CommandQueue& commands);
    void update_children(sf::Time dt, CommandQueue& commands);
    void draw_bounding_rect(sf::RenderTarget& target, sf::RenderStates states)
        const;

    std::vector<Ptr> m_children;
    SceneNode* m_parent;
    Category::Type m_default_category;
};

bool collision(const SceneNode& lhs, const SceneNode& rhs);
float distance(const SceneNode& lhs, const SceneNode& rhs);
