
#include <SFML/Graphics.hpp>
#include <math.h>
#include "ballBounceScroller.h"
#include "pointCloud.h"
#include "cube.h"
#include "sphere.h"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1024u, 768u}), "Oldfarts vol.4, N0lan 2025", sf::Style::None, sf::State::Windowed);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);

    auto scroller = BallBounceScroller(window);
    auto pointCloud = PointCloud();
    pointCloud.generatePoints(25600, static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));    
    Cube cube(800.f, sf::Vector2f(512.f, 384.f)); // Centered below text
    Sphere sphere(400.f, sf::Vector2f(512.f, 584.f)); // Centered below text
    sf::Clock clock;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }
        }

    
        window.clear();

        pointCloud.update(1.f/30.f);
        pointCloud.draw(window);
        
        scroller.update(1.f);
        scroller.draw(window);

        float dt = clock.getElapsedTime().asSeconds();
        sphere.rotate(dt*0.015f, dt * 0.04f, dt * 0.06f);
        sphere.move(sin(clock.getElapsedTime().asSeconds() * 3.0f) * 100.f, cos(clock.getElapsedTime().asSeconds() * 2.5f) * 100.f);
        sphere.draw(window);

        sphere.updateBallBounce(cube.getPosition(), cube.getVertices());
        cube.rotate(dt*0.01f, dt * 0.07f, dt * 0.05f);
        cube.draw(window);

        window.display();
    }
}
