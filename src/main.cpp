#include <SFML/Graphics.hpp>
#include <math.h>
#include "ballBounceScroller.h"
#include "pointCloud.h"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1024u}), "Oldfarts vol.4, N0lan 2025", sf::Style::None, sf::State::Fullscreen);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);

    auto scroller = BallBounceScroller(window);
    auto pointCloud = PointCloud();
    pointCloud.generatePoints(10000, static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
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
        window.display();
    }
}
