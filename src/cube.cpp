#include "cube.h"
#include <cmath>
#include <SFML/Graphics.hpp>

Cube::Cube(float size, sf::Vector2f center)
    : size(size), center(center), rotX(0), rotY(0), rotZ(0)
{
    float s = size / 2.f;
    vertices = {
        sf::Vector3f(-s, -s, -s), sf::Vector3f(s, -s, -s),
        sf::Vector3f(s, s, -s), sf::Vector3f(-s, s, -s),
        sf::Vector3f(-s, -s, s), sf::Vector3f(s, -s, s),
        sf::Vector3f(s, s, s), sf::Vector3f(-s, s, s)
    };
    edges = {
        std::make_pair(0,1), std::make_pair(1,2), std::make_pair(2,3), std::make_pair(3,0),
        std::make_pair(4,5), std::make_pair(5,6), std::make_pair(6,7), std::make_pair(7,4),
        std::make_pair(0,4), std::make_pair(1,5), std::make_pair(2,6), std::make_pair(3,7)
    };
    updateProjection();
}

void Cube::rotate(float angleX, float angleY, float angleZ) {
    rotX += angleX;
    rotY += angleY;
    rotZ += angleZ;
    updateProjection();
}

void Cube::updateProjection() {
    float radX = rotX, radY = rotY, radZ = rotZ;
    for (size_t i = 0; i < vertices.size(); ++i) {
        sf::Vector3f v = vertices[i];
        // Rotate X
        float y1 = v.y * cos(radX) - v.z * sin(radX);
        float z1 = v.y * sin(radX) + v.z * cos(radX);
        // Rotate Y
        float x2 = v.x * cos(radY) + z1 * sin(radY);
        float z2 = -v.x * sin(radY) + z1 * cos(radY);
        // Rotate Z
        float x3 = x2 * cos(radZ) - y1 * sin(radZ);
        float y3 = x2 * sin(radZ) + y1 * cos(radZ);
        // Simple perspective projection
        float fov = 400, viewerDist = 3 * size;
        float factor = fov / (viewerDist - z2);
        projected[i] = sf::Vector2f(center.x + x3 * factor, center.y + y3 * factor);
    }
}

void Cube::draw(sf::RenderWindow& window) {
    for (const auto& e : edges) {
        sf::Vertex line[2];
        line[0].position = projected[e.first];
        line[0].color = sf::Color::Red;
        line[1].position = projected[e.second];
        line[1].color = sf::Color::Red;
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }
}
