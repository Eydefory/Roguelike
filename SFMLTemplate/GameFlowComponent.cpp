#include "GameFlowComponent.h"

#include "AudioManager.h"
#include "Constants.h"
#include "DeveloperLevel.h"
#include "GameWorld.h"
#include "HealthComponent.h"
#include "InventoryHudComponent.h"
#include "Loger.h"
#include "Player.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <algorithm>
#include <cmath>
#include <string>

namespace XYZRoguelike
{
    namespace
    {
        constexpr float MENU_BUTTON_WIDTH = 400.f;
        constexpr float MENU_BUTTON_HEIGHT = 92.f;
        constexpr float MENU_BUTTON_GAP = 105.f;
        constexpr float VOLUME_TRACK_X = 490.f;
        constexpr float VOLUME_TRACK_WIDTH = 300.f;
    } // namespace

    GameFlowComponent::GameFlowComponent(XYZEngine::GameObject* gameObject,
                                         DeveloperLevel* level)
        : Component(gameObject), level(level)
    {
        backdrop.setSize(sf::Vector2f(static_cast<float>(SCREEN_WIDTH),
                                      static_cast<float>(SCREEN_HEIGHT)));
        backdrop.setFillColor(sf::Color(5, 7, 14, 250));

        if (const auto* texture =
                XYZEngine::ResourceSystem::Instance()->GetTextureShared("inventory_panel"))
        {
            constexpr float MENU_PANEL_WIDTH = 940.f;
            const auto textureSize = texture->getSize();
            const float scale = MENU_PANEL_WIDTH / static_cast<float>(textureSize.x);
            menuPanel.setTexture(*texture);
            menuPanel.setScale(scale, scale);
            menuPanel.setPosition((SCREEN_WIDTH - MENU_PANEL_WIDTH) * 0.5f,
                                  (SCREEN_HEIGHT - textureSize.y * scale) * 0.5f);
            menuPanelLoaded = true;
        }
        if (const auto* texture =
                XYZEngine::ResourceSystem::Instance()->GetTextureShared("title_background"))
        {
            titleBackground.setTexture(*texture);
            titleBackground.setScale(static_cast<float>(SCREEN_WIDTH) / texture->getSize().x,
                                     static_cast<float>(SCREEN_HEIGHT) / texture->getSize().y);
            titleBackgroundLoaded = true;
        }
        if (const auto* texture =
                XYZEngine::ResourceSystem::Instance()->GetTextureShared("victory_background"))
        {
            victoryBackground.setTexture(*texture);
            victoryBackground.setScale(static_cast<float>(SCREEN_WIDTH) / texture->getSize().x,
                                        static_cast<float>(SCREEN_HEIGHT) / texture->getSize().y);
            victoryBackgroundLoaded = true;
        }
        if (const auto* texture =
                XYZEngine::ResourceSystem::Instance()->GetTextureShared("defeat_background"))
        {
            defeatBackground.setTexture(*texture);
            defeatBackground.setScale(static_cast<float>(SCREEN_WIDTH) / texture->getSize().x,
                                      static_cast<float>(SCREEN_HEIGHT) / texture->getSize().y);
            defeatBackgroundLoaded = true;
        }
        auto* resources = XYZEngine::ResourceSystem::Instance();
        normalButtonTexture = resources->GetTextureShared("button_container");
        hoverButtonTexture = resources->GetTextureShared("button_container_hover");
        if (normalButtonTexture == nullptr)
        {
            normalButtonTexture = hoverButtonTexture;
        }
        if (normalButtonTexture != nullptr && hoverButtonTexture != nullptr)
        {
            buttonContainerLoaded = true;
        }
        if (const auto* texture = resources->GetTextureShared("end_screen_container"))
        {
            screenContainer.setTexture(*texture);
            constexpr float SCREEN_CONTAINER_HEIGHT = 500.f;
            const float scale = SCREEN_CONTAINER_HEIGHT / static_cast<float>(texture->getSize().y);
            screenContainer.setScale(scale, scale);
            screenContainer.setPosition((SCREEN_WIDTH - texture->getSize().x * scale) * 0.5f,
                                        (SCREEN_HEIGHT - texture->getSize().y * scale) * 0.5f);
            screenContainerLoaded = true;
        }
        if (const auto* texture = resources->GetTextureShared("pause_menu"))
        {
            pausePanel.setTexture(*texture);
            constexpr float PAUSE_PANEL_WIDTH = 850.f;
            const float scale = PAUSE_PANEL_WIDTH / static_cast<float>(texture->getSize().x);
            pausePanel.setScale(scale, scale);
            pausePanel.setPosition((SCREEN_WIDTH - texture->getSize().x * scale) * 0.5f,
                                   (SCREEN_HEIGHT - texture->getSize().y * scale) * 0.5f);
            pausePanelLoaded = true;
        }
        if (!screenContainerLoaded)
        {
            LOG_ERROR("Game screen container texture could not be loaded");
            panelFallback.setSize(sf::Vector2f(580.f, 420.f));
            panelFallback.setPosition(350.f, 145.f);
            panelFallback.setFillColor(sf::Color(16, 19, 32, 248));
            panelFallback.setOutlineThickness(0.f);
        }

        fontLoaded = font.loadFromFile("Resources/Fonts/Saltblackcombat-Regular.ttf");
        if (!fontLoaded)
        {
            LOG_ERROR("Game menu font could not be loaded");
        }
        else
        {
            title.setFont(font);
            title.setCharacterSize(50);
            title.setFillColor(sf::Color(245, 239, 255));

            subtitle.setFont(font);
            subtitle.setCharacterSize(30);
            subtitle.setFillColor(sf::Color(193, 188, 211));

            volumeLabel.setFont(font);
            volumeLabel.setCharacterSize(22);
            volumeLabel.setFillColor(sf::Color(221, 219, 234));
            volumeLabel.setString("VOLUME");

            volumeValue.setFont(font);
            volumeValue.setCharacterSize(20);
            volumeValue.setFillColor(sf::Color(221, 219, 234));
        }

        volumeTrack.setSize(sf::Vector2f(VOLUME_TRACK_WIDTH, 12.f));
        volumeTrack.setPosition(VOLUME_TRACK_X, 565.f);
        volumeTrack.setFillColor(sf::Color(32, 35, 56));
        volumeFill.setPosition(VOLUME_TRACK_X, 565.f);
        volumeFill.setSize(sf::Vector2f(0.f, 12.f));
        volumeFill.setFillColor(sf::Color(107, 206, 238));
        volumeKnob.setRadius(13.f);
        volumeKnob.setOrigin(13.f, 13.f);
        volumeKnob.setFillColor(sf::Color(214, 234, 255));
        volumeKnob.setOutlineColor(sf::Color(115, 190, 239));
        volumeKnob.setOutlineThickness(2.f);
        UpdateVolumeSlider(VOLUME_TRACK_X + VOLUME_TRACK_WIDTH *
                                                AudioManager::Instance().GetVolume() / 100.f);

        for (std::size_t index = 0; index < buttons.size(); ++index)
        {
            auto& button = buttons[index];
            button.shape.setSize(sf::Vector2f(MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT));
            button.shape.setPosition(440.f, 365.f + index * MENU_BUTTON_GAP);
            button.shape.setFillColor(sf::Color::Transparent);
            button.shape.setOutlineThickness(0.f);
            if (buttonContainerLoaded)
            {
                button.container.setTexture(*normalButtonTexture);
                const float scale = MENU_BUTTON_WIDTH / 1926.f;
                button.container.setScale(scale, scale);
                button.container.setPosition(button.shape.getPosition().x - 124.f * scale,
                                             button.shape.getPosition().y - 140.f * scale);
            }
            if (fontLoaded)
            {
                button.label.setFont(font);
                button.label.setCharacterSize(25);
                button.label.setFillColor(sf::Color(244, 240, 255));
            }
        }

        SetScreen(ScreenState::MainMenu);
        XYZEngine::GameWorld::Instance()->SetPaused(true);
    }

    void GameFlowComponent::Update(float deltaTime)
    {
        const bool escapePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
        bool inventoryOpen = false;
        if (screen == ScreenState::Playing && level != nullptr && level->GetPlayer() != nullptr)
        {
            const auto* inventoryHud =
                level->GetPlayer()->GetGameObject()->GetComponent<InventoryHudComponent>();
            inventoryOpen = inventoryHud != nullptr && inventoryHud->IsOpen();
        }
        const bool pausePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::P) ||
                                  (escapePressed &&
                                   (screen == ScreenState::Pause ||
                                    (screen == ScreenState::Playing && !inventoryOpen)));

        if (screen == ScreenState::Playing)
        {
            if (level != nullptr && level->IsRunComplete())
            {
                SetScreen(ScreenState::Victory);
                XYZEngine::GameWorld::Instance()->SetPaused(true);
                return;
            }

            if (level != nullptr && level->GetPlayer() != nullptr)
            {
                const auto* health = level->GetPlayer()->GetHealthComponent();
                if (health != nullptr && !health->IsAlive())
                {
                    SetScreen(ScreenState::Defeat);
                    XYZEngine::GameWorld::Instance()->SetPaused(true);
                    return;
                }
            }

            if (pausePressed && !pauseKeyWasPressed)
            {
                SetScreen(ScreenState::Pause);
                XYZEngine::GameWorld::Instance()->SetPaused(true);
            }
            pauseKeyWasPressed = pausePressed;
            return;
        }

        if (screen == ScreenState::Pause && pausePressed && !pauseKeyWasPressed)
        {
            SetScreen(ScreenState::Playing);
            XYZEngine::GameWorld::Instance()->SetPaused(false);
            pauseKeyWasPressed = pausePressed;
            return;
        }

        pauseKeyWasPressed = pausePressed;
        UpdateMenuInput();
    }

    void GameFlowComponent::Render()
    {
        if (screen == ScreenState::Playing)
        {
            return;
        }

        auto& window = XYZEngine::RenderSystem::Instance()->GetMainWindow();
        const auto previousView = window.getView();
        window.setView(window.getDefaultView());

        if (screen == ScreenState::Pause)
        {
            backdrop.setFillColor(sf::Color(4, 5, 14, 170));
            window.draw(backdrop);
            if (pausePanelLoaded)
            {
                window.draw(pausePanel);
            }
            else
            {
                window.draw(menuPanelLoaded ? menuPanel : screenContainer);
            }
            if (fontLoaded)
            {
                window.draw(title);
                window.draw(subtitle);
                window.draw(volumeLabel);
                window.draw(volumeValue);
            }
            for (std::size_t index = 0; index < buttons.size(); ++index)
            {
                auto& button = buttons[index];
                if (buttonContainerLoaded)
                {
                    button.container.setTexture(index == focusedButton ? *hoverButtonTexture
                                                                         : *normalButtonTexture);
                    window.draw(button.container);
                }
                else
                {
                    button.shape.setFillColor(index == focusedButton
                                                  ? sf::Color(79, 67, 126)
                                                  : sf::Color(35, 37, 57));
                    window.draw(button.shape);
                }
                if (fontLoaded)
                {
                    window.draw(button.label);
                }
            }
            window.draw(volumeTrack);
            window.draw(volumeFill);
            window.draw(volumeKnob);
            window.setView(previousView);
            return;
        }

        bool hasScreenBackground = false;
        if (screen == ScreenState::MainMenu && titleBackgroundLoaded)
        {
            window.draw(titleBackground);
            hasScreenBackground = true;
        }
        else if (screen == ScreenState::Victory && victoryBackgroundLoaded)
        {
            window.draw(victoryBackground);
            hasScreenBackground = true;
        }
        else if (screen == ScreenState::Defeat && defeatBackgroundLoaded)
        {
            window.draw(defeatBackground);
            hasScreenBackground = true;
        }
        if (!hasScreenBackground)
        {
            window.draw(backdrop);
            if (menuPanelLoaded)
            {
                window.draw(menuPanel);
            }
            else
            {
                window.draw(panelFallback);
            }
        }
        if (screenContainerLoaded)
        {
            window.draw(screenContainer);
        }
        if (fontLoaded)
        {
            window.draw(title);
            window.draw(subtitle);
        }

        for (std::size_t index = 0; index < buttons.size(); ++index)
        {
            auto& button = buttons[index];
            const bool focused = index == focusedButton;
            if (buttonContainerLoaded)
            {
                button.container.setTexture(focused ? *hoverButtonTexture : *normalButtonTexture);
                window.draw(button.container);
            }
            else
            {
                button.shape.setFillColor(focused ? sf::Color(79, 67, 126)
                                                  : sf::Color(35, 37, 57));
                window.draw(button.shape);
            }
            if (fontLoaded)
            {
                window.draw(button.label);
            }
        }

        window.setView(previousView);
    }

    void GameFlowComponent::SetScreen(ScreenState newScreen)
    {
        screen = newScreen;
        focusedButton = 0;

        const bool pauseScreen = screen == ScreenState::Pause;
        focusedButton = pauseScreen ? 1 : 0;
        for (std::size_t index = 0; index < buttons.size(); ++index)
        {
            auto& button = buttons[index];
            button.shape.setPosition(440.f, (pauseScreen ? 300.f : 365.f) +
                                                index * MENU_BUTTON_GAP);
            if (buttonContainerLoaded)
            {
                const float scale = MENU_BUTTON_WIDTH / 1926.f;
                button.container.setPosition(button.shape.getPosition().x - 124.f * scale,
                                             button.shape.getPosition().y - 140.f * scale);
            }
        }

        if (!fontLoaded)
        {
            return;
        }

        switch (screen)
        {
        case ScreenState::MainMenu:
            title.setString("XYZ ROGUELIKE");
            subtitle.setString("Three maze levels. Find the key and reach the exit.");
            buttons[0].label.setString("START RUN");
            buttons[1].label.setString("EXIT GAME");
            break;
        case ScreenState::Victory:
            title.setString("VICTORY!");
            subtitle.setString("You cleared every level of the maze.");
            buttons[0].label.setString("PLAY AGAIN");
            buttons[1].label.setString("MAIN MENU");
            break;
        case ScreenState::Defeat:
            title.setString("YOU DIED");
            subtitle.setString("The maze got you. Try another run.");
            buttons[0].label.setString("RESTART");
            buttons[1].label.setString("MAIN MENU");
            break;
        case ScreenState::Pause:
            title.setString("PAUSED");
            subtitle.setString("Take a breath, adventurer.");
            buttons[0].label.setString("MAIN MENU");
            buttons[1].label.setString("RESUME");
            CenterText(volumeLabel, SCREEN_WIDTH * 0.5f, 510.f);
            break;
        case ScreenState::Playing:
            return;
        }

        CenterText(title, SCREEN_WIDTH * 0.5f, pauseScreen ? 158.f : 226.f);
        CenterText(subtitle, SCREEN_WIDTH * 0.5f, pauseScreen ? 225.f : 302.f);
        for (auto& button : buttons)
        {
            CenterButtonLabel(button);
        }
    }

    void GameFlowComponent::UpdateMenuInput()
    {
        if (screen == ScreenState::Pause)
        {
            UpdatePauseInput();
            return;
        }

        auto& window = XYZEngine::RenderSystem::Instance()->GetMainWindow();
        const auto mouse = sf::Mouse::getPosition(window);
        const sf::Vector2f mousePosition(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
        for (std::size_t index = 0; index < buttons.size(); ++index)
        {
            if (buttons[index].shape.getGlobalBounds().contains(mousePosition))
            {
                focusedButton = index;
            }
        }

        const bool upPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
                               sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        const bool downPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
                                 sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
        const bool confirmPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) ||
                                    sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
        if (upPressed && !upWasPressed)
        {
            focusedButton = (focusedButton + buttons.size() - 1) % buttons.size();
        }
        if (downPressed && !downWasPressed)
        {
            focusedButton = (focusedButton + 1) % buttons.size();
        }
        if (confirmPressed && !confirmWasPressed)
        {
            ActivateButton(focusedButton);
        }

        const bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        if (mousePressed && !mouseWasPressed)
        {
            for (std::size_t index = 0; index < buttons.size(); ++index)
            {
                if (buttons[index].shape.getGlobalBounds().contains(mousePosition))
                {
                    focusedButton = index;
                    ActivateButton(index);
                    break;
                }
            }
        }

        upWasPressed = upPressed;
        downWasPressed = downPressed;
        confirmWasPressed = confirmPressed;
        mouseWasPressed = mousePressed;
    }

    void GameFlowComponent::UpdatePauseInput()
    {
        auto& window = XYZEngine::RenderSystem::Instance()->GetMainWindow();
        const auto mouse = sf::Mouse::getPosition(window);
        const sf::Vector2f mousePosition(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
        for (std::size_t index = 0; index < buttons.size(); ++index)
        {
            if (buttons[index].shape.getGlobalBounds().contains(mousePosition))
            {
                focusedButton = index;
            }
        }

        const bool upPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
                               sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        const bool downPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
                                 sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
        const bool confirmPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) ||
                                    sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
        if (upPressed && !upWasPressed)
        {
            focusedButton = (focusedButton + buttons.size() - 1) % buttons.size();
        }
        if (downPressed && !downWasPressed)
        {
            focusedButton = (focusedButton + 1) % buttons.size();
        }
        if (confirmPressed && !confirmWasPressed)
        {
            ActivateButton(focusedButton);
        }

        const bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        if (mousePressed && volumeTrack.getGlobalBounds().contains(mousePosition))
        {
            volumeDragging = true;
        }
        if (!mousePressed)
        {
            volumeDragging = false;
        }
        if (volumeDragging)
        {
            UpdateVolumeSlider(mousePosition.x);
        }

        if (mousePressed && !mouseWasPressed)
        {
            for (std::size_t index = 0; index < buttons.size(); ++index)
            {
                if (buttons[index].shape.getGlobalBounds().contains(mousePosition))
                {
                    focusedButton = index;
                    ActivateButton(index);
                    break;
                }
            }
        }

        const bool leftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
                                 sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        const bool rightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
                                  sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        if (leftPressed && !leftWasPressed)
        {
            AudioManager::Instance().SetVolume(AudioManager::Instance().GetVolume() - 5.f);
        }
        if (rightPressed && !rightWasPressed)
        {
            AudioManager::Instance().SetVolume(AudioManager::Instance().GetVolume() + 5.f);
        }
        if ((leftPressed && !leftWasPressed) || (rightPressed && !rightWasPressed))
        {
            UpdateVolumeSlider(VOLUME_TRACK_X +
                               VOLUME_TRACK_WIDTH * AudioManager::Instance().GetVolume() / 100.f);
        }

        upWasPressed = upPressed;
        downWasPressed = downPressed;
        confirmWasPressed = confirmPressed;
        mouseWasPressed = mousePressed;
        leftWasPressed = leftPressed;
        rightWasPressed = rightPressed;
    }

    void GameFlowComponent::UpdateVolumeSlider(float mouseX)
    {
        const float ratio = std::clamp((mouseX - VOLUME_TRACK_X) / VOLUME_TRACK_WIDTH, 0.f, 1.f);
        AudioManager::Instance().SetVolume(ratio * 100.f);
        volumeFill.setSize(sf::Vector2f(VOLUME_TRACK_WIDTH * ratio, 12.f));
        volumeKnob.setPosition(VOLUME_TRACK_X + VOLUME_TRACK_WIDTH * ratio, 571.f);
        volumeValue.setString(std::to_string(static_cast<int>(std::lround(ratio * 100.f))) + "%");
        CenterText(volumeValue, 830.f, 535.f);
    }

    void GameFlowComponent::ActivateButton(std::size_t index)
    {
        if (index >= buttons.size() || actionPending)
        {
            return;
        }

        if (screen == ScreenState::MainMenu && index == 1)
        {
            XYZEngine::RenderSystem::Instance()->GetMainWindow().close();
            return;
        }

        if (screen == ScreenState::Pause)
        {
            if (index == 0)
            {
                QueueReturnToMenu();
            }
            else
            {
                SetScreen(ScreenState::Playing);
                XYZEngine::GameWorld::Instance()->SetPaused(false);
            }
            return;
        }

        if (screen == ScreenState::MainMenu || screen == ScreenState::Victory ||
            screen == ScreenState::Defeat)
        {
            if (index == 0)
            {
                QueueRestart();
            }
            else if (screen != ScreenState::MainMenu)
            {
                SetScreen(ScreenState::MainMenu);
                XYZEngine::GameWorld::Instance()->SetPaused(true);
            }
        }
    }

    void GameFlowComponent::QueueRestart()
    {
        if (level == nullptr)
        {
            return;
        }

        actionPending = true;
        auto* root = GetGameObject();
        XYZEngine::GameWorld::Instance()->ScheduleAfterUpdate(
            [this, root]()
            {
                level->RestartKeeping(root);
                SetScreen(ScreenState::Playing);
                actionPending = false;
                XYZEngine::GameWorld::Instance()->SetPaused(false);
            });
    }

    void GameFlowComponent::QueueReturnToMenu()
    {
        if (level == nullptr)
        {
            return;
        }

        actionPending = true;
        auto* root = GetGameObject();
        XYZEngine::GameWorld::Instance()->ScheduleAfterUpdate(
            [this, root]()
            {
                level->ReturnToMenuKeeping(root);
                SetScreen(ScreenState::MainMenu);
                actionPending = false;
                XYZEngine::GameWorld::Instance()->SetPaused(true);
            });
    }

    void GameFlowComponent::CenterText(sf::Text& text, float centerX, float y)
    {
        const auto bounds = text.getLocalBounds();
        text.setPosition(centerX - (bounds.left + bounds.width * 0.5f), y);
    }

    void GameFlowComponent::CenterButtonLabel(MenuButton& button)
    {
        const auto bounds = button.label.getLocalBounds();
        const auto position = button.shape.getPosition();
        const auto size = button.shape.getSize();
        button.label.setPosition(position.x + (size.x - bounds.width) * 0.5f - bounds.left,
                                 position.y + (size.y - bounds.height) * 0.5f - bounds.top);
    }
} // namespace XYZRoguelike
