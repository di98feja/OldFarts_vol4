#include "sphere.h"
#include <cmath>
#include <SFML/Graphics.hpp>

Sphere::Sphere(float size, sf::Vector2f center)
    : size(size), center(center), rotX(0), rotY(0), rotZ(0)
{
    float s = size / 2.f;
    center = center;
    vertices = std::vector<sf::Vector3f>{};
    int num_lines = 16;
    for (int i = 0; i <= num_lines; i++) {
        auto linestrip = std::vector<int>{};
        float theta = i == 0 || i == num_lines ? 0.1f : i * 2 * M_PI / num_lines;
        for (int j = 0; j <= num_lines; j++) {
            float phi = j == 0 || j == num_lines ? 0.1f : j * 2 * M_PI / num_lines;
            float x = s * sin(theta) * cos(phi);
            float y = s * sin(theta) * sin(phi);
            float z = s * cos(theta);
            vertices.push_back(sf::Vector3f(x, y, z));
            linestrip.push_back(vertices.size() - 1);
        }
        edges.push_back(linestrip);
    }
    for (int i = 0; i <= num_lines + 1; i++) 
    {
        auto linestrip = std::vector<int>{};
        for (int j = 0; j < num_lines; j++) 
        {
          linestrip.push_back(i + j * (num_lines + 1));
        }
        edges.push_back(linestrip);    
    }
    updateProjection();
}

void Sphere::rotate(float angleX, float angleY, float angleZ) {
    rotX = angleX;
    rotY = angleY;
    rotZ = angleZ;
}



void Sphere::updateProjection() {
    float radX = rotX, radY = rotY, radZ = rotZ;
    projected.clear();
    projected.resize(vertices.size());
    for (size_t i = 0; i < vertices.size(); i++) {
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
        projected[i] = sf::Vector2f(center.x + pos.x + x3 * factor, center.y + pos.y + y3 * factor);
    }
}

void Sphere::draw(sf::RenderWindow& window) {
  updateProjection();
   for (const auto& e : edges) {
        std::vector<sf::Vertex> line;
        for (const auto& i : e) 
        {
            sf::Vertex p;
            p.position = projected[i];
            p.color = sf::Color::Red;
            line.push_back(p);
        }
        window.draw(line.data(), line.size(), sf::PrimitiveType::LineStrip);
   }
}

void Sphere::move(float dx, float dy)
{
    pos = sf::Vector2f(dx, dy);
}

bool Sphere::isInside(sf::Vector2f point)
{
    float dx = point.x - (center.x + pos.x);
    float dy = point.y - (center.y + pos.y);
    float distanceSquared = dx * dx + dy * dy;
    float radius = size / 2.f;
    return distanceSquared <= radius * radius;
}

sf::Vector2f Sphere::pushOut(sf::Vector3f point, sf::Vector3f angle)
{
    // Calculate the direction vector from the sphere's center to the point
    sf::Vector3f dir = point - sf::Vector3f(center.x + pos.x, center.y + pos.y, 0);
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length == 0) return sf::Vector2f();

    // Normalize the direction vector
    dir /= length;

    // Calculate the push-out distance
    float pushOutDistance = (size / 2.f) - length;

    // Apply the push-out along the direction vector
    return sf::Vector2f(dir.x * pushOutDistance, dir.y * pushOutDistance);
}

void Sphere::updateBallBounce(sf::Vector2f cubePosition, std::vector<sf::Vector3f>& cubeVertices)
{
    for (const auto &cubeVertex : cubeVertices) {
      if (isInside(sf::Vector2f(cubePosition.x + cubeVertex.x, cubePosition.y + cubeVertex.y))) 
      {
          sf::Vector2f d = pushOut(cubeVertex,  sf::Vector3f(center.x, center.y, 0.0f) - sf::Vector3f(pos.x, pos.y, 0.0f));
          // Apply the push-out to the sphere's position
          pos += d;
      }
    }
}
