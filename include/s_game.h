#pragma once

//#define SFML_STATIC

#include "state.h"
#include "world.h"
#include "player.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class GameState : public State {
public:
    GameState(StateStack& stack, Context context);
    ~GameState();

    virtual void draw();
    virtual bool update(sf::Time delta_time);
    virtual bool handle_event(const sf::Event& event);
    // rendering handled by app
private:
    World m_world;
    Player& m_player; // player& -> (may be) already loaded in mem
};
