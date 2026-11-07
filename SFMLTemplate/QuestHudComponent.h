#pragma once

#include "Component.h"
#include "InventoryComponent.h"

#include <SFML/Graphics.hpp>

#include <array>

namespace XYZRoguelike
{
    class EnemyContainer;

    class QuestHudComponent : public XYZEngine::Component
    {
    public:
        explicit QuestHudComponent(XYZEngine::GameObject* gameObject);

        void Update(float deltaTime) override;
        void Render() override;
        bool IsOverlay() const override
        {
            return true;
        }

        void SetEnemyContainer(EnemyContainer* container);
        void SetLevelNumber(int levelNumber);

    private:
        void RefreshText();

        EnemyContainer* enemies = nullptr;
        InventoryComponent* inventory = nullptr;
        sf::Font font;
        sf::Sprite panel;
        sf::Text title;
        std::array<sf::Text, 3> objectives;
        std::array<sf::Text, 3> statuses;
        int currentLevel = 1;
        bool fontLoaded = false;
    };
} // namespace XYZRoguelike
