#pragma once
#include <SFML/Graphics.hpp>
#include <array>

class Cube {
public:
    Cube(float size, sf::Vector2f center);
    void rotate(float angleX, float angleY, float angleZ);
    void draw(sf::RenderWindow& window);
private:
    std::array<sf::Vector3f, 8> vertices;
    std::array<std::pair<int, int>, 12> edges;
    sf::Vector2f center;
    float size;
    float rotX, rotY, rotZ;
    std::array<sf::Vector2f, 8> projected;
    void updateProjection();
};
