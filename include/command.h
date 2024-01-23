#pragma once

#include "category.h"

#include <SFML/System/Time.hpp>

#include <functional>
#include <cassert>

class SceneNode;

struct Command {
    Command();
    // treat function as object
    std::function<void(SceneNode& node, sf::Time dt)> action;
    // store the recipients of the command in a category
    unsigned int category;
};

template <typename GameObject, typename Function>
std::function<void(SceneNode&, sf::Time)> derived_action(Function fn)
{
    return [=] (SceneNode& node, sf::Time dt) {
        // check if cast is safe
        assert(dynamic_cast<GameObject*>(&node) != nullptr);
        // downcast node and invoke function on it
        fn(static_cast<GameObject&>(node), dt);
    };
}
