#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

#include "GameState.h"

namespace XYZRoguelike
{
    class Game
    {
    public:
        Game();

        void Run();

    private:
        enum class GameScreen
        {
            Menu,
            Playing,
            Win,
            GameOver
        };

        sf::RenderWindow window;
        GameState gameState;
        sf::Font font;

        GameScreen currentScreen;

        std::vector<int> records;
        bool scoreRecorded;

        void HandleEvents();
        void Update(float deltaTime);
        void Draw();

        void DrawMenu();
        void DrawGame();
        void DrawInterface();
        void DrawWin();
        void DrawGameOver();
        void DrawRecords(float startY);

        void AddRecord(int score);

        void StartGame();
        void RestartGame();
    };
}