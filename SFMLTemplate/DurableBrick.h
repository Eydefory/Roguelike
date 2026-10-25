#pragma once

#include "Brick.h"
#include <vector>

namespace XYZRoguelike
{
    class DurableBrick : public Brick
    {
    public:
        DurableBrick(float x, float y);

        void Draw(sf::RenderWindow& window) const override;
        void Destroy() override;

        int GetHitPoints() const;
        void SetHitPoints(int value);

    protected:
        void OnHit() override;

    private:
        int hitPoints;
        std::vector<sf::Color> damageColors;

        void UpdateColor();
    };
}