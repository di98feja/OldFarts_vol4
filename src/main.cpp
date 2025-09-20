
#include <SFML/Graphics.hpp>
#include <math.h>
#include "ballBounceScroller.h"
#include "pointCloud.h"
#include "cube.h"


int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1024u}), "Oldfarts vol.4, N0lan 2025", sf::Style::None, sf::State::Windowed);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);

    auto scroller = BallBounceScroller(window);
    auto pointCloud = PointCloud();
    pointCloud.generatePoints(25600, static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));    
    Cube cube(200.f, sf::Vector2f(960.f, 700.f)); // Centered below text
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

        float dt = clock.restart().asSeconds();
        cube.rotate(dt, dt * 0.7f, dt * 0.5f);
        cube.draw(window);

        window.display();
    }
}
