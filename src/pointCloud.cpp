#include "pointCloud.h"
#include "noiseGenerator.h"

void PointCloud::generatePixels(float width, float height)
{
    this->pixels = sf::VertexArray(sf::PrimitiveType::Points);
    NoiseGenerator noise;
    for (int y = 0; y < height * 2.f; ++y)
    {
        for (int x = 0; x < width * 2.f; ++x)
        {
            // Scale coordinates to get interesting patterns
            float fx = static_cast<float>(x) * 0.02f;
            float fy = static_cast<float>(y) * 0.02f; 
            float noiseValue = noise.fbm(fx, fy,  this->timer.getElapsedTime().asSeconds()*0.1f);
            noiseValue = (noiseValue + 1.0f) * 0.5f;  // Convert from [-1,1] to [0,1]
            noiseValue = std::max(0.0f, std::min(1.0f, noiseValue)); // Clamp
            uint8_t colorValue = static_cast<uint8_t>(1.f-noiseValue * 255);
            float amplitudeValue = noise.fbm(fy, fx,  this->timer.getElapsedTime().asSeconds()*0.15f);
            amplitudeValue = (amplitudeValue + 1.0f) * 0.5f;  // Convert from [-1,1] to [0,1]
            amplitudeValue = std::max(0.0f, std::min(1.0f, amplitudeValue)); // Clamp
            this->pixels.append(sf::Vertex{sf::Vector2f(static_cast<float>(x), static_cast<float>(y)), sf::Color(colorValue, colorValue, static_cast<uint8_t>(1.f-amplitudeValue * 255))});
        }
    }
}

void PointCloud::generatePoints(size_t numPoints, float width, float height)
{
    this->width = width;
    this->height = height;

    this->points = sf::VertexArray(sf::PrimitiveType::Points);
//    this->points.resize(numPoints);

    this->originalPositions = std::vector<sf::Vector2f>();
    this->originalPositions.reserve(numPoints);
    
    // Calculate grid dimensions
    float aspectRatio = width / height;
    this->point_cols = static_cast<size_t>(std::sqrt(numPoints * aspectRatio));
    this->point_rows = (numPoints + this->point_cols - 1) / this->point_cols;  // Round up division

    this->generatePixels(this->point_cols, this->point_rows);
 
    // Calculate spacing
    float xSpacing = width / static_cast<float>(this->point_cols);
    float ySpacing = height / static_cast<float>(this->point_rows);
    
    // Generate grid points
    for (size_t row = 0; row < this->point_rows && this->originalPositions.size() < numPoints; ++row) {
        for (size_t col = 0; col < this->point_cols && this->originalPositions.size() < numPoints; ++col) {
            float x = (col + 0.5f) * xSpacing ;  // +0.5f centers the points
            float y = (row + 0.5f) * ySpacing ;
            this->originalPositions.emplace_back(x, y);
        }
    }
    for (const auto& point : this->originalPositions)
    {
      this->points.append(sf::Vertex{point, sf::Color::White});
    }
}

void PointCloud::update(float deltaTime)
{
  //generatePixels(this->width, this->height);
    // move points in a sin&cos wave pattern
   // SinCosWavePattern(deltaTime);

    // Alternatively, use NoiseGenerator
   fbmPattern(deltaTime);

    //    NoiseGenerator noise;
    
    // Generate noise for a 2D texture
    // for (int y = 0; y < this->height; ++y) {
    //     for (int x = 0; x < this->width; ++x) {
    //         // Scale coordinates to get interesting patterns
    //         float fx = static_cast<float>(x) * 0.01f;
    //         float fy = static_cast<float>(y) * 0.01f;
            
    //         float noiseValue = noise.fbm(fx, fy, 0.0f);
            
    //         // noiseValue is roughly in range [0, 1]
    //     }
    // }
}

void PointCloud::SinCosWavePattern(float deltaTime)
{
  for (int i = 0; i < this->points.getVertexCount(); ++i)
  {
    auto &point = this->points[i].position;
    point.x += std::sin(point.y * 0.0015f + deltaTime) * std::sin(this->timer.getElapsedTime().asMilliseconds() * 0.0002f) * 20.5f;
    point.y += std::cos(point.x * 0.0013f + deltaTime) * std::cos(this->timer.getElapsedTime().asMilliseconds() * 0.0003f) * 25.5f;

    this->points[i].color = sf::Color(
        static_cast<uint8_t>(148 + 107 * std::sin(this->timer.getElapsedTime().asMilliseconds() * 0.002f + point.x * 0.01f)),
        static_cast<uint8_t>(148 + 107 * std::sin(this->timer.getElapsedTime().asMilliseconds() * 0.003f + point.y * 0.01f)),
        static_cast<uint8_t>(148 + 107 * std::sin(this->timer.getElapsedTime().asMilliseconds() * 0.004f + (point.x + point.y) * 0.01f)));

    // Wrap around the screen edges
    if (point.x < 0)
      point.x += this->width;
    if (point.x >= this->width)
      point.x -= this->width;
    if (point.y < 0)
      point.y += this->height;
    if (point.y >= this->height)
      point.y -= this->height;
  }
}

void PointCloud::fbmPattern(float deltaTime)
{
//  pixel_start_col = (size_t)this->timer.getElapsedTime().asSeconds() % this->point_cols;
  if (this->pixel_start_col == 0 || this->pixel_start_col == this->point_cols-1)
  {
    pixel_increment = -pixel_increment;
  }
  this->pixel_start_col += pixel_increment;

  this->generatePixels(this->point_cols, this->point_rows);
  size_t pixelIndex = 0;   //this->pixel_start_col;

  for (int i = 0; i < this->points.getVertexCount(); ++i)
  {
    auto &point = this->originalPositions[i];
    auto noiseValues = this->pixels[pixelIndex%this->pixels.getVertexCount()].color;
    auto angle = noiseValues.r/256.f * 2.0f * 3.14159265f;
    this->points[i].position.x = point.x + sin(angle) * noiseValues.b * 10.0f;
    this->points[i].position.y = point.y + cos(angle) * noiseValues.b * 10.0f;

    pixelIndex++;

    if (pixelIndex%this->point_cols == 0)
    {
      pixelIndex += point_cols*3;
    }

    this->points[i].color = sf::Color(
        static_cast<uint8_t>(128 + 127 * std::sin(this->timer.getElapsedTime().asMilliseconds() * 0.002f + point.x * 0.01f)),
        static_cast<uint8_t>(128 + 127 * std::sin(this->timer.getElapsedTime().asMilliseconds() * 0.003f + point.y * 0.01f)),
        static_cast<uint8_t>(128 + 127 * std::sin(this->timer.getElapsedTime().asMilliseconds() * 0.004f + (point.x + point.y) * 0.01f)));

    // Wrap around the screen edges
    if (point.x < 0)
      point.x += this->width;
    if (point.x >= this->width)
      point.x -= this->width;
    if (point.y < 0)
      point.y += this->height;
    if (point.y >= this->height)
      point.y -= this->height;
  }
}

void PointCloud::draw(sf::RenderWindow& window) const
{
    window.draw(this->points);
    window.draw(this->pixels);
}
