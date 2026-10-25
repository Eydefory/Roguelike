#pragma once

#include <SFML/Graphics.hpp>

namespace XYZRoguelike
{
    class Platform
    {
    public:
        Platform();

        void Reset();
        void ResetWidth();
        void Update(float deltaTime);
        void MoveToMouse(float mouseX);
        void Draw(sf::RenderWindow& window) const;

        sf::FloatRect GetBounds() const;
        float GetWidth() const;
        float GetPositionX() const;

        void SetPositionX(float x);
        void SetTemporaryWidth(float width);

    private:
        sf::RectangleShape shape;

        float speed;
        float startX;
        float startY;

        void ClampToScreen();
    };
}