#include "ballBounceScroller.h"

BallBounceScroller::BallBounceScroller(sf::RenderWindow& window)
{
    this->windowWidth = window.getSize().x;
    this->timer = sf::Clock();
    this->balls = initializeBalls(window);
    this->line = initializeLine(window);
    this->font = sf::Font(FONT_PATH);
    this->leftmost_char_sprite = createCharSprite(0);
}

BallBounceScroller::~BallBounceScroller()
{
}

std::array<Ball, BallBounceScroller::NUM_BALLS> BallBounceScroller::initializeBalls(sf::RenderWindow& window)
{
    std::array<Ball, BallBounceScroller::NUM_BALLS> balls;
    for (auto& ball : balls)
    {
        ball.shape = sf::CircleShape();
        ball.shape.setRadius(BALL_RADIUS);
        ball.shape.setFillColor(sf::Color::Green);
        ball.shape.setPosition({static_cast<float>(rand() % (window.getSize().x - static_cast<unsigned int>(BALL_RADIUS * 2))), this->LINE_Y - BALL_RADIUS * 2.f - 50.f});
        ball.x_speed = static_cast<float>(BALL_X_SPEED_MIN + rand() % static_cast<int>(BALL_X_SPEED_MAX - BALL_X_SPEED_MIN));
        if (rand() % 2 == 0)
            ball.x_speed = -ball.x_speed;
        ball.y_speed = static_cast<float>(5 + rand() % static_cast<int>(3));
    }
    return balls;
}

sf::VertexArray BallBounceScroller::initializeLine(sf::RenderWindow& window)
{
    sf::VertexArray line;
    line.setPrimitiveType(sf::PrimitiveType::LineStrip);
    for (float f = 0.f; f < window.getSize().x; f += 2.f)
    {
        line.append(sf::Vertex{sf::Vector2f(f, this->LINE_Y), sf::Color::White});
    }
    return line;
}

CharSprite* BallBounceScroller::createCharSprite(unsigned int scrolltext_index)
{
    auto cs = new CharSprite();
    cs->texture = std::make_unique<sf::RenderTexture>(sf::Vector2u(this->FONT_SIZE, this->FONT_SIZE));
    cs->texture->draw(sf::Text(this->font, this->SCROLL_TEXT[scrolltext_index], this->FONT_SIZE));
    cs->texture->display();
    cs->sprite = std::make_unique<sf::Sprite>(cs->texture->getTexture());
    cs->line_index = this->line.getVertexCount() - 1;
    cs->scrolltext_index = scrolltext_index;
    return cs;
}

void BallBounceScroller::update(float deltaTime)
{
    // Update ball positions
    for (auto& ball : balls)
    {
        float baseY = this->LINE_Y;
        ball.shape.setPosition({ball.shape.getPosition().x + ball.x_speed, baseY + sin(this->timer.getElapsedTime().asSeconds()  * ball.y_speed) * 150.f});
//        ball.shape.move({ball.x_speed, sin(this->timer.getElapsedTime().asSeconds() * ball.y_speed) * 7.f});
        if (ball.shape.getPosition().x < 0.f || ball.shape.getPosition().x + BALL_RADIUS * 2.f > this->windowWidth)
        {
            ball.x_speed = -ball.x_speed;
        }
    }

    // Reset line to base Y
    for (size_t i = 0; i < line.getVertexCount(); ++i)
    {
        line[i].position.y = this->LINE_Y;
    }    

    // Calculate ball effect on line
    for (const auto& ball : balls)
    {
        auto amplitude = (this->LINE_Y > ball.shape.getPosition().y + BALL_RADIUS * 2.f ? this->LINE_Y : ball.shape.getPosition().y + BALL_RADIUS * 2.f) - this->LINE_Y;
        auto sigma = 300.f;
        for (size_t i = 0; i < line.getVertexCount(); ++i)
        {
            auto& vertex = line[i];
            float distance = vertex.position.x - (ball.shape.getPosition().x + BALL_RADIUS);
            float newY = this->LINE_Y + amplitude * exp(-(distance * distance) / (2 * sigma * sigma));
            if (newY > vertex.position.y)
            {
              vertex.position.y = this->LINE_Y + amplitude * exp(-(distance * distance) / (2 * sigma * sigma));
            }
        }
    }

    // Update sprite positions
    updateSpritePositions(this->leftmost_char_sprite);
}

void BallBounceScroller::updateSpritePositions(CharSprite*& cs)
{
    if (cs == nullptr) return;

    CharSprite* rightmost = nullptr;
    for (auto* current = cs; current != nullptr; current = current->next)
    {
        if (current->line_index > 0)
        {
            current->line_index--;
            current->sprite->setPosition(line[current->line_index].position);
            rightmost = current;
            // Calculate sprite rotation based on line slope
            if (current->line_index < line.getVertexCount() - 1)
            {
                float dy = line[current->line_index + 1].position.y - line[current->line_index].position.y;
                float dx = line[current->line_index + 1].position.x - line[current->line_index].position.x;
                auto angle = sf::radians(atan2(dy, dx));
                current->sprite->setRotation(angle);
            }
        }
    }
    if (cs->line_index == 0)
    {
        auto* toDelete = cs;
        cs = cs->next;
        delete toDelete;
    }
    if (rightmost != nullptr)
    {
        if (rightmost->sprite->getPosition().x < this->windowWidth - this->FONT_SIZE * 0.8f)
        {
            auto* newCharSprite = createCharSprite((rightmost->scrolltext_index + 1) % SCROLL_TEXT.size());
            newCharSprite->line_index = this->line.getVertexCount() - 1;
            newCharSprite->sprite->setPosition(line[newCharSprite->line_index].position);
            newCharSprite->scrolltext_index = (rightmost->scrolltext_index + 1) % SCROLL_TEXT.size();
            rightmost->next = newCharSprite;
        }
    }
}

void BallBounceScroller::draw(sf::RenderWindow& window)
{
    window.draw(line);

    for (const auto& ball : balls)
    {
        window.draw(ball.shape);
    }

    for (auto* current = leftmost_char_sprite; current != nullptr; current = current->next)
    {
        window.draw(*current->sprite);
    }
}