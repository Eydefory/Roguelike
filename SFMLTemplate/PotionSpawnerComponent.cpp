#include "PotionSpawnerComponent.h"

#include "Constants.h"
#include "GameWorld.h"
#include "InventoryHudComponent.h"

#include <algorithm>
#include <cmath>
#include <utility>

namespace XYZRoguelike
{
    PotionSpawnerComponent::PotionSpawnerComponent(XYZEngine::GameObject* gameObject)
        : Component(gameObject), randomEngine(std::random_device{}())
    {
    }

    void PotionSpawnerComponent::Update(float deltaTime)
    {
        auto* hud = gameObject->GetComponent<InventoryHudComponent>();
        if (hud != nullptr && hud->IsOpen())
        {
            return;
        }

        if (!std::isfinite(deltaTime) || deltaTime <= 0.f || spawnPositions.empty() ||
            !spawnCallback)
        {
            return;
        }

        timeUntilNextSpawn -= deltaTime;
        if (timeUntilNextSpawn > 0.f)
        {
            return;
        }

        timeUntilNextSpawn = 20.f;
        SpawnRandomPotion();
    }

    void PotionSpawnerComponent::Render() {}

    void PotionSpawnerComponent::SetSpawnPoints(std::vector<sf::Vector2f> positions,
                                                SpawnCallback callback)
    {
        spawnPositions = std::move(positions);
        spawnCallback = std::move(callback);
        timeUntilNextSpawn = 20.f;
    }

    void PotionSpawnerComponent::SpawnRandomPotion()
    {
        std::uniform_int_distribution<std::size_t> positionDistribution(0,
                                                                        spawnPositions.size() - 1);
        std::uniform_int_distribution<int> itemDistribution(0, 2);
        const sf::Vector2f position = spawnPositions[positionDistribution(randomEngine)];

        ItemType type = ItemType::HealingPotion;
        switch (itemDistribution(randomEngine))
        {
        case 1:
            type = ItemType::ShieldPotion;
            break;
        case 2:
            type = ItemType::StaminaPotion;
            break;
        default:
            break;
        }

        const SpawnCallback callback = spawnCallback;
        XYZEngine::GameWorld::Instance()->ScheduleAfterUpdate([callback, type, position]()
                                                              { callback(type, position); });
    }
} // namespace XYZRoguelike
