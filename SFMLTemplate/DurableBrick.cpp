#include "DurableBrick.h"
#include <algorithm>

namespace XYZRoguelike
{
    DurableBrick::DurableBrick(float x, float y)
        : Brick(x, y),
        hitPoints(3),
        damageColors{
            sf::Color(120, 180, 255),
            sf::Color(255, 190, 80),
            sf::Color(220, 80, 100)
        }
    {
        UpdateColor();
    }

    void DurableBrick::OnHit()
    {
        if (destroyed)
            return;

        --hitPoints;

        if (hitPoints <= 0)
        {
            Destroy();
        }
        else
        {
            UpdateColor();
        }
    }

    void DurableBrick::Destroy()
    {
        hitPoints = 0;
        destroyed = true;
    }

    void DurableBrick::UpdateColor()
    {
        int colorIndex = 3 - hitPoints;
        colorIndex = std::max(0, std::min(colorIndex, 2));

        brickShape.setFillColor(damageColors[colorIndex]);
    }

    int DurableBrick::GetHitPoints() const
    {
        return hitPoints;
    }

    void DurableBrick::SetHitPoints(int value)
    {
        hitPoints = std::max(0, std::min(value, 3));
        destroyed = hitPoints == 0;

        if (!destroyed)
            UpdateColor();
    }

    void DurableBrick::Draw(sf::RenderWindow& window) const
    {
        if (!destroyed)
            window.draw(brickShape);
    }
}