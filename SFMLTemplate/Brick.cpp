#include "Brick.h"
#include "Constants.h"

namespace XYZRoguelike
{
    Brick::Brick(float x, float y)
        : destroyed(false)
    {
        brickShape.setSize(sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT));
        brickShape.setPosition(x, y);
        brickShape.setFillColor(sf::Color(220, 80, 100));
    }

    void Brick::Update(float deltaTime)
    {
    }

    void Brick::Draw(sf::RenderWindow& window) const
    {
        if (!destroyed)
            window.draw(brickShape);
    }

    sf::FloatRect Brick::GetBounds() const
    {
        return brickShape.getGlobalBounds();
    }

    bool Brick::CheckCollision(const sf::FloatRect& ballBounds)
    {
        if (destroyed)
            return false;

        if (!ballBounds.intersects(brickShape.getGlobalBounds()))
            return false;

        OnHit();
        return true;
    }

    void Brick::OnHit()
    {
        Destroy();
    }

    void Brick::Destroy()
    {
        destroyed = true;
    }

    bool Brick::IsDestroyed() const
    {
        return destroyed;
    }
}