#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
class Cube {
public:
  Cube(float size, sf::Vector2f center);
  void CalcSide(sf::Vector3f &corner2, sf::Vector3f &corner1, sf::Vector3f &corner4, std::vector<sf::Vector3f> &vertices);
  void NewFunction(float s, std::vector<sf::Vector3f> &vertices);
  void verticesOnSide(std::vector<sf::Vector3f> &vertices, float x, float y, float z, int numSegments = 4);
  void rotate(float angleX, float angleY, float angleZ);
  void draw(sf::RenderWindow &window);
  std::vector<sf::Vector3f>& getVertices() { return vertices; }
  sf::Vector2f getPosition() { return center; }
private:
    std::vector<sf::Vector3f> vertices;
    std::vector<std::vector<int>> edges;
    sf::Vector2f center;
    float size;
    float rotX, rotY, rotZ;
    std::vector<sf::Vector2f> projected;
    void updateProjection();
};
