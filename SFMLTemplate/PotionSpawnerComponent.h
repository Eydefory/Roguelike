#pragma once

#include "Component.h"
#include "InventoryComponent.h"

#include <SFML/System/Vector2.hpp>
#include <functional>
#include <random>
#include <vector>

namespace XYZRoguelike
{
    class PotionSpawnerComponent : public XYZEngine::Component
    {
    public:
        using SpawnCallback = std::function<void(ItemType, sf::Vector2f)>;

        explicit PotionSpawnerComponent(XYZEngine::GameObject* gameObject);

        void Update(float deltaTime) override;
        void Render() override;
        void SetSpawnPoints(std::vector<sf::Vector2f> positions, SpawnCallback callback);

    private:
        void SpawnRandomPotion();

        std::vector<sf::Vector2f> spawnPositions;
        SpawnCallback spawnCallback;
        std::mt19937 randomEngine;
        float timeUntilNextSpawn = 20.f;
    };
} // namespace XYZRoguelike
