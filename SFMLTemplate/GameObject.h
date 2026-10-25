#pragma once

#include <SFML/Graphics.hpp>

namespace XYZRoguelike
{
    class GameObject
    {
    public:
        virtual ~GameObject() = default;

        virtual void Update(float deltaTime) = 0;
        virtual void Draw(sf::RenderWindow& window) const = 0;
        virtual sf::FloatRect GetBounds() const = 0;
    };
}