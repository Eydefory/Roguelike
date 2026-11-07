#pragma once

#include "Component.h"

#include <SFML/Graphics.hpp>

#include <array>
#include <cstddef>

namespace XYZRoguelike
{
    class DeveloperLevel;

    class GameFlowComponent : public XYZEngine::Component
    {
    public:
        GameFlowComponent(XYZEngine::GameObject* gameObject, DeveloperLevel* level);

        void Update(float deltaTime) override;
        void Render() override;
        bool IsOverlay() const override
        {
            return true;
        }

    private:
        enum class ScreenState
        {
            MainMenu,
            Playing,
            Pause,
            Victory,
            Defeat
        };

        struct MenuButton
        {
            sf::RectangleShape shape;
            sf::Sprite container;
            sf::Text label;
        };

        void SetScreen(ScreenState screen);
        void UpdateMenuInput();
        void UpdatePauseInput();
        void UpdateVolumeSlider(float mouseX);
        void ActivateButton(std::size_t index);
        void QueueRestart();
        void QueueReturnToMenu();
        void CenterText(sf::Text& text, float centerX, float y);
        void CenterButtonLabel(MenuButton& button);

        DeveloperLevel* level = nullptr;
        ScreenState screen = ScreenState::MainMenu;
        sf::Font font;
        bool fontLoaded = false;
        bool menuPanelLoaded = false;
        bool victoryBackgroundLoaded = false;
        bool defeatBackgroundLoaded = false;
        bool buttonContainerLoaded = false;
        bool screenContainerLoaded = false;
        bool upWasPressed = false;
        bool downWasPressed = false;
        bool confirmWasPressed = false;
        bool mouseWasPressed = false;
        bool pauseKeyWasPressed = false;
        bool leftWasPressed = false;
        bool rightWasPressed = false;
        bool volumeDragging = false;
        bool actionPending = false;
        std::size_t focusedButton = 0;

        sf::RectangleShape backdrop;
        sf::Sprite menuPanel;
        sf::Sprite titleBackground;
        bool titleBackgroundLoaded = false;
        sf::Sprite victoryBackground;
        sf::Sprite defeatBackground;
        sf::Sprite screenContainer;
        sf::Sprite pausePanel;
        bool pausePanelLoaded = false;
        const sf::Texture* normalButtonTexture = nullptr;
        const sf::Texture* hoverButtonTexture = nullptr;
        sf::RectangleShape panelFallback;
        sf::RectangleShape volumeTrack;
        sf::RectangleShape volumeFill;
        sf::CircleShape volumeKnob;
        sf::Text title;
        sf::Text subtitle;
        sf::Text volumeLabel;
        sf::Text volumeValue;
        std::array<MenuButton, 2> buttons;
    };
} // namespace XYZRoguelike
