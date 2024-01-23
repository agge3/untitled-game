#define SFML_STATIC

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "testing");
    window.setFramerateLimit(60);
    window.setView(window.getDefaultView());
    if (!ImGui::SFML::Init(window))
        return 1;
    ImGui::StyleColorsLight();
    ImVec2 sfml_view(window.getView().getSize().x,
            window.getView().getSize().y);
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    ImVec2 pos(0.f, 0.f);

    sf::Clock delta_clock;

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed)
                window.close();
        }
        ImGui::SFML::Update(window, delta_clock.restart());
        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowSize(sfml_view);
        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button!");
        ImGui::End();

        window.clear();
        window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
}
