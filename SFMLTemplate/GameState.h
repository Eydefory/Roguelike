#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <string>
#include <vector>

#include "Ball.h"
#include "Brick.h"
#include "GameMemento.h"
#include "Platform.h"
#include "ScoreStrategy.h"

namespace XYZRoguelike
{
    class Bonus;

    class GameState
    {
    public:
        GameState();
        ~GameState();

        void Reset();
        void Update(float deltaTime);
        void HandleMouse(float mouseX);

        void Draw(sf::RenderWindow& window) const;

        bool IsGameOver() const;
        bool IsWin() const;

        int GetScore() const;
        int GetLives() const;

        void ApplyFireballBonus();
        void ApplyShrinkBonus();
        void ApplySpeedBonus();

        GameMemento Save() const;
        void Load(const GameMemento& memento);

        bool SaveToFile(const std::string& fileName) const;
        bool LoadFromFile(const std::string& fileName);

    private:
        Platform platform;
        Ball ball;

        std::vector<std::unique_ptr<Brick>> bricks;
        std::vector<std::unique_ptr<Bonus>> bonuses;

        std::unique_ptr<ScoreStrategy> scoreStrategy;

        int score;
        int lives;

        bool gameOver;
        bool win;

        float fireballTimer;
        float shrinkTimer;
        float speedTimer;

        void CreateBricks();
        void CheckBrickCollisions();
        void CheckBonuses();

        void SpawnBonus(float x, float y);

        bool AllBricksDestroyed() const;

        void LoseLife();
        void UpdateBonusEffects(float deltaTime);
    };
}