#include "GameMemento.h"

namespace XYZRoguelike
{
    GameMemento::GameMemento(
        int scoreValue,
        int livesValue,
        const sf::Vector2f& ballPositionValue,
        float platformXValue,
        float platformWidthValue,
        float fireballTimerValue,
        float shrinkTimerValue,
        float speedTimerValue,
        const std::vector<bool>& destroyedBricksValue,
        const std::vector<int>& brickHitPointsValue
    )
        : score(scoreValue),
        lives(livesValue),
        ballPosition(ballPositionValue),
        platformX(platformXValue),
        platformWidth(platformWidthValue),
        fireballTimer(fireballTimerValue),
        shrinkTimer(shrinkTimerValue),
        speedTimer(speedTimerValue),
        destroyedBricks(destroyedBricksValue),
        brickHitPoints(brickHitPointsValue)
    {
    }

    int GameMemento::GetScore() const
    {
        return score;
    }

    int GameMemento::GetLives() const
    {
        return lives;
    }

    sf::Vector2f GameMemento::GetBallPosition() const
    {
        return ballPosition;
    }

    float GameMemento::GetPlatformX() const
    {
        return platformX;
    }

    float GameMemento::GetPlatformWidth() const
    {
        return platformWidth;
    }

    float GameMemento::GetFireballTimer() const
    {
        return fireballTimer;
    }

    float GameMemento::GetShrinkTimer() const
    {
        return shrinkTimer;
    }

    float GameMemento::GetSpeedTimer() const
    {
        return speedTimer;
    }

    const std::vector<bool>& GameMemento::GetDestroyedBricks() const
    {
        return destroyedBricks;
    }

    const std::vector<int>& GameMemento::GetBrickHitPoints() const
    {
        return brickHitPoints;
    }
}