#pragma once

#include "Component.h"
#include "HealthComponent.h"

#include <SFML/Graphics.hpp>

namespace XYZRoguelike
{
    class HealthBarComponent : public XYZEngine::Component
    {
    public:
        HealthBarComponent(XYZEngine::GameObject* gameObject);

        void Update(float deltaTime) override;
        void Render() override;

    private:
        sf::RectangleShape background;
        sf::RectangleShape healthBar;
        sf::RectangleShape armorBar;

        HealthComponent* healthComponent;

        void UpdateBars();
    };
}