#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class PointCloud {
public:
    PointCloud() = default;
    ~PointCloud() = default;

    void generatePoints(size_t numPoints, float width, float height);
    void generatePixels(float width, float height);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
private:
    sf::Clock timer = sf::Clock();
    std::vector<sf::Vector2f> originalPositions;
    sf::VertexArray points;
    sf::VertexArray pixels;
    float pointSize = 1.0f;
    float width = 0.0f;
    float height = 0.0f;
    size_t point_cols = 0.0f;
    size_t point_rows = 0.0f;
    size_t pixel_start_col = 0;
    size_t pixel_increment = 1;
    void SinCosWavePattern(float deltaTime);
    void fbmPattern(float deltaTime);
};