#pragma once

//#define SFML_STATIC

#include <state.h>
#include <p_task.h>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class LoadingState : public State {
public:
    LoadingState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time delta_time);
    virtual bool handle_event(const sf::Event& event);

    void set_completion(float percent);
private:
    sf::Text m_loading_text;
    sf::RectangleShape m_progress_bar_background;
    sf::RectangleShape m_progress_bar;

    ParallelTask m_loading_task;
};
