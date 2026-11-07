#pragma once

#include "Component.h"
#include "HealthComponent.h"
#include "StaminaComponent.h"

#include <SFML/Graphics.hpp>

#include <array>

namespace XYZRoguelike
{
    class HealthBarComponent : public XYZEngine::Component
    {
    public:
        HealthBarComponent(XYZEngine::GameObject* gameObject);

        void Update(float deltaTime) override;
        void Render() override;
        void SetLevelNumber(int levelNumber);
        bool IsOverlay() const override
        {
            return true;
        }

    private:
        sf::Sprite panel;
        sf::Sprite healthBarFrame;
        sf::Sprite healthFill;
        sf::Sprite shieldIcon;
        std::array<sf::Sprite, 5> shieldIcons;
        std::array<sf::RectangleShape, 3> meterFrames;
        std::array<sf::RectangleShape, 3> meterFills;

        HealthComponent* healthComponent;
        StaminaComponent* staminaComponent;
        sf::Font font;
        std::array<sf::Text, 3> meterLabels;
        sf::Text title;
        int currentLevel = 1;
        bool fontLoaded = false;

        void UpdateBars();
    };
} // namespace XYZRoguelike
