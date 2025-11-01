#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
class Sphere {
public:
  Sphere(float size, sf::Vector2f center);
  void rotate(float angleX, float angleY, float angleZ);
  void draw(sf::RenderWindow &window);
  void move(float dx, float dy);
  bool isInside(sf::Vector2f point);
  sf::Vector2f pushOut(sf::Vector3f point, sf::Vector3f angle);
  void updateBallBounce(sf::Vector2f cubePosition, std::vector<sf::Vector3f>& cubeVertices);
private:
    float M_PI = 3.14159265358979323846;
    std::vector<sf::Vector3f> vertices;
    std::vector<std::vector<int>> edges;
    sf::Vector2f center;
    sf::Vector2f pos;
    float size;
    float rotX, rotY, rotZ;
    std::vector<sf::Vector2f> projected;
    void updateProjection();
};
