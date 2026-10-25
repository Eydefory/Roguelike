#include "Bonus.h"

#include "BonusCommand.h"
#include "Constants.h"
#include "GameState.h"

namespace XYZRoguelike
{
    Bonus::Bonus(
        float x,
        float y,
        BonusType bonusType,
        std::unique_ptr<BonusCommand> bonusCommand
    )
        : type(bonusType),
        command(std::move(bonusCommand)),
        collected(false)
    {
        shape.setSize(
            sf::Vector2f(BONUS_SIZE, BONUS_SIZE)
        );

        shape.setPosition(x, y);

        if (type == BonusType::Fireball)
        {
            shape.setFillColor(sf::Color::Red);
        }
        else if (type == BonusType::Shrink)
        {
            shape.setFillColor(sf::Color::Yellow);
        }
        else
        {
            shape.setFillColor(sf::Color::Cyan);
        }
    }

    void Bonus::Update(float deltaTime)
    {
        if (!collected)
            shape.move(
                0.f,
                BONUS_FALL_SPEED * deltaTime
            );
    }

    void Bonus::Draw(sf::RenderWindow& window) const
    {
        if (!collected)
            window.draw(shape);
    }

    sf::FloatRect Bonus::GetBounds() const
    {
        return shape.getGlobalBounds();
    }

    bool Bonus::IsCollected() const
    {
        return collected;
    }

    bool Bonus::IsOutOfScreen() const
    {
        return shape.getPosition().y > SCREEN_HEIGHT;
    }

    void Bonus::Collect(GameState& gameState)
    {
        if (collected)
            return;

        collected = true;

        if (command)
            command->Execute(gameState);
    }
}