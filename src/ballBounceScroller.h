#pragma once
#include <SFML/Graphics.hpp>

struct Ball
{
    sf::CircleShape shape;
    float x_speed = 0.f;
    float y_speed = 0.f;
};

struct CharSprite
{
    std::unique_ptr<sf::Sprite> sprite = nullptr;
    std::unique_ptr<sf::RenderTexture> texture = nullptr;
    unsigned int line_index;
    unsigned int scrolltext_index;
    CharSprite* next = nullptr;
};

class BallBounceScroller
{
public:
    BallBounceScroller(sf::RenderWindow& window);
    ~BallBounceScroller();

    static const unsigned int NUM_BALLS = 3u;
    const float BALL_RADIUS = 20.f;
    const float BALL_X_SPEED_MAX = 7.f;
    const float BALL_X_SPEED_MIN = 3.f;
    const float LINE_Y = 300.f;
    const std::string FONT_PATH = "./gfx/Fake Receipt.otf";
    const std::string SCROLL_TEXT = "N0LAN 2025 - THE OLD FARTS VOL.4    IN THEATERS EVERYWHERE - COMING SOON             ";
    const unsigned int FONT_SIZE = 45u;

    unsigned int windowWidth = 0u;

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

private:
    std::array<Ball, NUM_BALLS> balls;
    CharSprite* leftmost_char_sprite;
    sf::Clock timer;
    sf::VertexArray line;
    sf::Font font;
    sf::RenderTexture texture;
    unsigned int sprite_line_index;

    std::array<Ball, NUM_BALLS> initializeBalls(sf::RenderWindow& window);
    sf::VertexArray initializeLine(sf::RenderWindow& window);
    CharSprite* createCharSprite(unsigned int scrolltext_index);
    void updateSpritePositions(CharSprite*& cs);
};