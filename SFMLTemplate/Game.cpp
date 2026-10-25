#include "Game.h"

#include "Constants.h"

#include <algorithm>
#include <string>

namespace XYZRoguelike
{
    Game::Game()
        : window(
            sf::VideoMode(
                SCREEN_WIDTH,
                SCREEN_HEIGHT
            ),
            "Arkanoid"
        ),
        currentScreen(GameScreen::Menu),
        scoreRecorded(false)
    {
        window.setFramerateLimit(60);

        font.loadFromFile(
            "Resources/Fonts/Roboto-Light.ttf"
        );
    }

    void Game::Run()
    {
        sf::Clock clock;

        while (window.isOpen())
        {
            float deltaTime =
                clock.restart().asSeconds();

            HandleEvents();
            Update(deltaTime);
            Draw();
        }
    }

    void Game::HandleEvents()
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                continue;
            }

            if (event.type == sf::Event::MouseMoved &&
                currentScreen == GameScreen::Playing)
            {
                gameState.HandleMouse(
                    static_cast<float>(
                        event.mouseMove.x
                        )
                );
            }

            if (event.type != sf::Event::KeyPressed)
                continue;

            if (event.key.code == sf::Keyboard::Escape)
            {
                window.close();
                continue;
            }

            if (event.key.code == sf::Keyboard::F5 &&
                currentScreen == GameScreen::Playing)
            {
                gameState.SaveToFile("save.txt");
                continue;
            }

            if (event.key.code == sf::Keyboard::F9)
            {
                if (gameState.LoadFromFile("save.txt"))
                {
                    currentScreen =
                        GameScreen::Playing;

                    scoreRecorded = false;
                }

                continue;
            }

            if (currentScreen == GameScreen::Menu)
            {
                if (event.key.code ==
                    sf::Keyboard::Enter)
                {
                    StartGame();
                }
            }
            else if (currentScreen == GameScreen::Win)
            {
                if (event.key.code ==
                    sf::Keyboard::Y)
                {
                    RestartGame();
                }

                if (event.key.code ==
                    sf::Keyboard::N)
                {
                    currentScreen =
                        GameScreen::Menu;
                }
            }
            else if (currentScreen ==
                GameScreen::GameOver)
            {
                if (event.key.code ==
                    sf::Keyboard::Enter)
                {
                    RestartGame();
                }

                if (event.key.code ==
                    sf::Keyboard::Escape)
                {
                    currentScreen =
                        GameScreen::Menu;
                }
            }
        }
    }

    void Game::Update(float deltaTime)
    {
        if (currentScreen != GameScreen::Playing)
            return;

        gameState.Update(deltaTime);

        if (gameState.IsWin())
        {
            currentScreen = GameScreen::Win;

            if (!scoreRecorded)
            {
                AddRecord(gameState.GetScore());
                scoreRecorded = true;
            }
        }
        else if (gameState.IsGameOver())
        {
            currentScreen = GameScreen::GameOver;

            if (!scoreRecorded)
            {
                AddRecord(gameState.GetScore());
                scoreRecorded = true;
            }
        }
    }

    void Game::Draw()
    {
        window.clear(
            sf::Color(20, 20, 35)
        );

        if (currentScreen == GameScreen::Menu)
            DrawMenu();
        else
            DrawGame();

        window.display();
    }

    void Game::DrawMenu()
    {
        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(50);
        title.setFillColor(sf::Color::White);
        title.setString("ARKANOID");

        sf::FloatRect titleBounds =
            title.getLocalBounds();

        title.setPosition(
            (SCREEN_WIDTH -
                titleBounds.width) / 2.f,
            150.f
        );

        window.draw(title);

        sf::Text start;
        start.setFont(font);
        start.setCharacterSize(26);
        start.setFillColor(sf::Color::White);
        start.setString("ENTER - Start");

        sf::FloatRect startBounds =
            start.getLocalBounds();

        start.setPosition(
            (SCREEN_WIDTH -
                startBounds.width) / 2.f,
            260.f
        );

        window.draw(start);

        sf::Text saveInfo;
        saveInfo.setFont(font);
        saveInfo.setCharacterSize(20);
        saveInfo.setFillColor(sf::Color::White);
        saveInfo.setString(
            "F9 - Load saved game"
        );

        sf::FloatRect saveBounds =
            saveInfo.getLocalBounds();

        saveInfo.setPosition(
            (SCREEN_WIDTH -
                saveBounds.width) / 2.f,
            310.f
        );

        window.draw(saveInfo);

        sf::Text exit;
        exit.setFont(font);
        exit.setCharacterSize(22);
        exit.setFillColor(sf::Color::White);
        exit.setString("ESC - Exit");

        sf::FloatRect exitBounds =
            exit.getLocalBounds();

        exit.setPosition(
            (SCREEN_WIDTH -
                exitBounds.width) / 2.f,
            360.f
        );

        window.draw(exit);

        DrawRecords(430.f);
    }

    void Game::DrawGame()
    {
        gameState.Draw(window);

        DrawInterface();

        if (currentScreen == GameScreen::Win)
            DrawWin();

        if (currentScreen == GameScreen::GameOver)
            DrawGameOver();
    }

    void Game::DrawInterface()
    {
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(22);
        scoreText.setFillColor(sf::Color::White);

        scoreText.setString(
            "Score: " +
            std::to_string(
                gameState.GetScore()
            )
        );

        scoreText.setPosition(20.f, 15.f);

        window.draw(scoreText);

        sf::Text livesText;
        livesText.setFont(font);
        livesText.setCharacterSize(22);
        livesText.setFillColor(sf::Color::White);

        livesText.setString(
            "Lives: " +
            std::to_string(
                gameState.GetLives()
            )
        );

        livesText.setPosition(680.f, 15.f);

        window.draw(livesText);

        sf::Text controls;
        controls.setFont(font);
        controls.setCharacterSize(14);
        controls.setFillColor(sf::Color::White);

        controls.setString(
            "F5 Save | F9 Load | "
            "Red=Fireball Yellow=Shrink Cyan=Speed"
        );

        controls.setPosition(175.f, 40.f);

        window.draw(controls);
    }

    void Game::DrawWin()
    {
        sf::RectangleShape overlay;

        overlay.setSize(
            sf::Vector2f(
                SCREEN_WIDTH,
                SCREEN_HEIGHT
            )
        );

        overlay.setFillColor(
            sf::Color(0, 0, 0, 190)
        );

        window.draw(overlay);

        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(45);
        title.setFillColor(sf::Color::Green);
        title.setString("YOU WIN!");

        sf::FloatRect titleBounds =
            title.getLocalBounds();

        title.setPosition(
            (SCREEN_WIDTH -
                titleBounds.width) / 2.f,
            100.f
        );

        window.draw(title);

        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(25);
        scoreText.setFillColor(sf::Color::White);

        scoreText.setString(
            "Score: " +
            std::to_string(
                gameState.GetScore()
            )
        );

        sf::FloatRect scoreBounds =
            scoreText.getLocalBounds();

        scoreText.setPosition(
            (SCREEN_WIDTH -
                scoreBounds.width) / 2.f,
            170.f
        );

        window.draw(scoreText);

        DrawRecords(220.f);

        sf::Text question;
        question.setFont(font);
        question.setCharacterSize(22);
        question.setFillColor(sf::Color::White);
        question.setString(
            "Y - Play again    N - Menu"
        );

        sf::FloatRect questionBounds =
            question.getLocalBounds();

        question.setPosition(
            (SCREEN_WIDTH -
                questionBounds.width) / 2.f,
            470.f
        );

        window.draw(question);
    }

    void Game::DrawGameOver()
    {
        sf::RectangleShape overlay;

        overlay.setSize(
            sf::Vector2f(
                SCREEN_WIDTH,
                SCREEN_HEIGHT
            )
        );

        overlay.setFillColor(
            sf::Color(0, 0, 0, 190)
        );

        window.draw(overlay);

        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(45);
        title.setFillColor(sf::Color::Red);
        title.setString("GAME OVER");

        sf::FloatRect titleBounds =
            title.getLocalBounds();

        title.setPosition(
            (SCREEN_WIDTH -
                titleBounds.width) / 2.f,
            100.f
        );

        window.draw(title);

        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(25);
        scoreText.setFillColor(sf::Color::White);

        scoreText.setString(
            "Score: " +
            std::to_string(
                gameState.GetScore()
            )
        );

        sf::FloatRect scoreBounds =
            scoreText.getLocalBounds();

        scoreText.setPosition(
            (SCREEN_WIDTH -
                scoreBounds.width) / 2.f,
            170.f
        );

        window.draw(scoreText);

        DrawRecords(220.f);

        sf::Text restart;
        restart.setFont(font);
        restart.setCharacterSize(22);
        restart.setFillColor(sf::Color::White);
        restart.setString(
            "ENTER - Restart"
        );

        sf::FloatRect restartBounds =
            restart.getLocalBounds();

        restart.setPosition(
            (SCREEN_WIDTH -
                restartBounds.width) / 2.f,
            470.f
        );

        window.draw(restart);
    }

    void Game::DrawRecords(float startY)
    {
        sf::Text title;
        title.setFont(font);
        title.setCharacterSize(24);
        title.setFillColor(sf::Color::White);
        title.setString("RECORDS");

        sf::FloatRect titleBounds =
            title.getLocalBounds();

        title.setPosition(
            (SCREEN_WIDTH -
                titleBounds.width) / 2.f,
            startY
        );

        window.draw(title);

        float y = startY + 35.f;

        for (std::size_t i = 0;
            i < records.size() && i < 5;
            ++i)
        {
            sf::Text record;
            record.setFont(font);
            record.setCharacterSize(18);
            record.setFillColor(sf::Color::White);

            record.setString(
                std::to_string(i + 1) +
                ". " +
                std::to_string(records[i])
            );

            sf::FloatRect bounds =
                record.getLocalBounds();

            record.setPosition(
                (SCREEN_WIDTH -
                    bounds.width) / 2.f,
                y
            );

            window.draw(record);

            y += 25.f;
        }
    }

    void Game::AddRecord(int score)
    {
        records.push_back(score);

        std::sort(
            records.begin(),
            records.end(),
            std::greater<int>()
        );

        if (records.size() > 5)
            records.resize(5);
    }

    void Game::StartGame()
    {
        gameState.Reset();

        currentScreen =
            GameScreen::Playing;

        scoreRecorded = false;
    }

    void Game::RestartGame()
    {
        gameState.Reset();

        currentScreen =
            GameScreen::Playing;

        scoreRecorded = false;
    }
}