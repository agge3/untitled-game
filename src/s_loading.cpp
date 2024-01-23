#include "s_loading.h"
#include "utility.h"
#include "r_holders.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

LoadingState::LoadingState(StateStack& stack, Context context) :
    State(stack, context)
{
    // get &render window from context
    sf::RenderWindow& window = *get_context().window;
    // get &font from context
    sf::Font& font = context.fonts->get(Fonts::Main);
    // set view to size of window
    sf::Vector2f view_size = window.getView().getSize();

    // loading text...
    m_loading_text.setFont(font);
    m_loading_text.setString("Loading...");
    center_origin(m_loading_text);
    // (x, y) px
    m_loading_text.setPosition(680, 600);

    // loading progress bar...
    m_progress_bar_background.setFillColor(sf::Color::White);
    m_progress_bar_background.setSize(sf::Vector2f(view_size.x - 20, 10));
    m_progress_bar_background.setPosition(10, m_loading_text.getPosition().y
            + 40);

    // loading progress bar % complete...
    m_progress_bar.setFillColor(sf::Color(100, 100, 100));
    m_progress_bar.setSize(sf::Vector2f(200, 10));
    m_progress_bar.setPosition(10, m_loading_text.getPosition().y + 40);

    // completion %
    set_completion(0.f);

    // exec loading as parallel task
    m_loading_task.execute();
}

void LoadingState::draw()
{
    // get window (already created) from context
    sf::RenderWindow& window = *get_context().window;
    // set window view to full screen (default view of window)
    window.setView(window.getDefaultView());
    // draw loading screen assets...
    window.draw(m_loading_text);
    window.draw(m_progress_bar_background);
    window.draw(m_progress_bar);
}

bool LoadingState::update(sf::Time delta_time)
{
    // loading -> delta time not needed, not game logic
    // update progress bar from parallel task or finish loading
    if (m_loading_task.is_finished()) {
        request_pop_stack();
        request_push_stack(States::Game);
    } else {
        set_completion(m_loading_task.get_completion());
    }

    return true;
}

bool LoadingState::handle_event(const sf::Event& event)
{
    // don't need to handle events -> loading
    return true;
}

void LoadingState::set_completion(float percent)
{
    // clamp to 100%
    if (percent > 1.f)
        percent = 1.f;

    // progress bar same x size as background, y based on loading %
    m_progress_bar.setSize(sf::Vector2f(m_progress_bar_background.getSize().x *
                percent, m_progress_bar.getSize().y));
}
