#pragma once

#include <SFML/Graphics.hpp>

namespace XYZRoguelike
{
    class Ball
    {
    public:
        Ball();

        void Reset();
        void Update(float deltaTime);
        void Draw(sf::RenderWindow& window) const;

        void BounceHorizontal();
        void BounceVertical();

        bool CheckPlatformCollision(const sf::FloatRect& platformBounds);
        bool IsOutOfScreen() const;

        sf::Vector2f GetPosition() const;
        sf::FloatRect GetBounds() const;

        void SetPosition(const sf::Vector2f& position);
        void SetSpeedMultiplier(float multiplier);
        void ResetSpeed();

    private:
        sf::CircleShape shape;
        sf::Vector2f velocity;

        float speed;
        float startX;
        float startY;
    };
}