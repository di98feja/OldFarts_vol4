#include "cube.h"
#include <cmath>
#include <SFML/Graphics.hpp>

Cube::Cube(float size, sf::Vector2f center)
    : size(size), center(center), rotX(0), rotY(0), rotZ(0)
{
    float s = size / 2.f;
    // vertices = {
    //     sf::Vector3f(-s, -s, -s), sf::Vector3f(s, -s, -s),
    //     sf::Vector3f(s, s, -s), sf::Vector3f(-s, s, -s),
    //     sf::Vector3f(-s, -s, s), sf::Vector3f(s, -s, s),
    //     sf::Vector3f(s, s, s), sf::Vector3f(-s, s, s)
    // };
    vertices = std::vector<sf::Vector3f>{};

    // Calculate sub squares of each side, ccw
    CalcSide(sf::Vector3f(s, -s, -s), sf::Vector3f(-s, -s, -s), sf::Vector3f(-s, s, -s), vertices);
    CalcSide(sf::Vector3f(s, s, -s), sf::Vector3f(s, -s, -s), sf::Vector3f(s, -s, s), vertices);
    CalcSide(sf::Vector3f(-s, s, -s), sf::Vector3f(s, s, -s), sf::Vector3f(s, s, s), vertices);
    CalcSide(sf::Vector3f(-s, -s, s), sf::Vector3f(-s, s, s), sf::Vector3f(s, s, s), vertices);
    CalcSide(sf::Vector3f(-s, -s, -s), sf::Vector3f(-s, -s, s), sf::Vector3f(-s, s, s), vertices);
    CalcSide(sf::Vector3f(s, -s, s), sf::Vector3f(s, -s, -s), sf::Vector3f(-s, -s, -s), vertices);

    updateProjection();
}

void Cube::CalcSide(sf::Vector3f &corner2, sf::Vector3f &corner1, sf::Vector3f &corner4, std::vector<sf::Vector3f> &vertices)
{
    auto xf = corner2 - corner1;
    auto yf = corner4 - corner1;
    int numSegments = 8;
    int verticesStartIndex = vertices.size();
    for (int i = 0; i <= numSegments; i++)
    {
        for (int j = 0; j <= numSegments; j++)
        {
            float fx = corner1.x + xf.x * (i / static_cast<float>(numSegments)) + yf.x * (j / static_cast<float>(numSegments));
            float fy = corner1.y + xf.y * (i / static_cast<float>(numSegments)) + yf.y * (j / static_cast<float>(numSegments));
            float fz = corner1.z + xf.z * (i / static_cast<float>(numSegments)) + yf.z * (j / static_cast<float>(numSegments));
            vertices.push_back(sf::Vector3f(fx, fy, fz));
        }
    }
    for (int i = 0; i <= numSegments; i++)
    {
        auto vLine = std::vector<int>();
        auto hLine = std::vector<int>();
        for (int j = 0; j <= numSegments; j++)
        {
            vLine.push_back(verticesStartIndex + i + j * (numSegments + 1));
            hLine.push_back(verticesStartIndex + i * (numSegments + 1) + j);
        }
        edges.push_back(vLine);
        edges.push_back(hLine);
    }
}


void Cube::rotate(float angleX, float angleY, float angleZ) {
    rotX = angleX;
    rotY = angleY;
    rotZ = angleZ;
    updateProjection();
}

void Cube::updateProjection() {
    float radX = rotX, radY = rotY, radZ = rotZ;
    projected.clear();
    projected.resize(vertices.size());
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
        float fov = size, viewerDist = 3 * size;
        float factor = fov / (viewerDist - z2);
        projected[i] = sf::Vector2f(center.x + x3 * factor, center.y + y3 * factor);
    }
}

void Cube::draw(sf::RenderWindow& window) {
   for (const auto& e : edges) {
        std::vector<sf::Vertex> line;
        for (const auto& i : e) 
        {
            sf::Vertex p;
            p.position = projected[i];
            p.color = sf::Color::Red;
            line.push_back(p);
        }
        window.draw(line.data(), 9, sf::PrimitiveType::LineStrip);
   }
}
