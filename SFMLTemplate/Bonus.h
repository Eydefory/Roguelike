#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

#include "GameObject.h"

namespace XYZRoguelike
{
    class GameState;
    class BonusCommand;

    enum class BonusType
    {
        Fireball,
        Shrink,
        Speed
    };

    class Bonus : public GameObject
    {
    public:
        Bonus(
            float x,
            float y,
            BonusType type,
            std::unique_ptr<BonusCommand> command
        );

        void Update(float deltaTime) override;
        void Draw(sf::RenderWindow& window) const override;
        sf::FloatRect GetBounds() const override;

        bool IsCollected() const;
        bool IsOutOfScreen() const;

        void Collect(GameState& gameState);

    private:
        sf::RectangleShape shape;

        BonusType type;
        std::unique_ptr<BonusCommand> command;

        bool collected;
    };
}