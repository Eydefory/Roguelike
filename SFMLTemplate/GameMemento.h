#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

namespace XYZRoguelike
{
    class GameMemento
    {
    public:
        GameMemento(
            int score,
            int lives,
            const sf::Vector2f& ballPosition,
            float platformX,
            float platformWidth,
            float fireballTimer,
            float shrinkTimer,
            float speedTimer,
            const std::vector<bool>& destroyedBricks,
            const std::vector<int>& brickHitPoints
        );

        int GetScore() const;
        int GetLives() const;

        sf::Vector2f GetBallPosition() const;

        float GetPlatformX() const;
        float GetPlatformWidth() const;

        float GetFireballTimer() const;
        float GetShrinkTimer() const;
        float GetSpeedTimer() const;

        const std::vector<bool>& GetDestroyedBricks() const;
        const std::vector<int>& GetBrickHitPoints() const;

    private:
        int score;
        int lives;

        sf::Vector2f ballPosition;

        float platformX;
        float platformWidth;

        float fireballTimer;
        float shrinkTimer;
        float speedTimer;

        std::vector<bool> destroyedBricks;
        std::vector<int> brickHitPoints;
    };
}