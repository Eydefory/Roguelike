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
        bool IsOverlay() const override
        {
            return true;
        }

    private:
        sf::Sprite panel;
        sf::Sprite healthBarFrame;
        sf::Sprite healthFill;
        sf::Sprite sprintBarFrame;
        sf::Sprite sprintFill;
        std::array<sf::Sprite, 5> shieldIcons;

        HealthComponent* healthComponent;
        StaminaComponent* staminaComponent;

        void UpdateBars();
    };
} // namespace XYZRoguelike
