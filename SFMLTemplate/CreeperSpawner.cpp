#include "CreeperSpawner.h"

#include "Creeper.h"
#include "EnemyContainer.h"
#include "Loger.h"
#include "TransformComponent.h"

#include <algorithm>
#include <cmath>
#include <random>
#include <stdexcept>
#include <string>

namespace XYZRoguelike
{
    CreeperSpawner::CreeperSpawner(CreeperSpawnSettings newSettings, EnemyFactory newFactory)
        : settings(newSettings), factory(std::move(newFactory))
    {
        if (!std::isfinite(settings.minimumPlayerDistance) || settings.minimumPlayerDistance < 0.f)
        {
            throw std::invalid_argument("Creeper minimum spawn distance must be non-negative");
        }

        if (!factory)
        {
            factory = [](XYZEngine::GameObject* player, float x, float y)
            { return std::make_unique<Creeper>(player, x, y); };
        }
    }

    unsigned int CreeperSpawner::SpawnCreepers(
        EnemyContainer& enemies, XYZEngine::GameObject* player,
        const std::vector<SpawnPoint>& walkablePositions) const
    {
        if (settings.count == 0)
        {
            return 0;
        }

        if (player == nullptr)
        {
            throw std::invalid_argument("Creeper spawner requires a player");
        }

        auto* playerTransform = player->GetComponent<XYZEngine::TransformComponent>();
        if (playerTransform == nullptr)
        {
            throw std::invalid_argument("Creeper spawner player has no TransformComponent");
        }

        const auto playerPosition = playerTransform->GetWorldPosition();
        const float minimumDistanceSquared =
            settings.minimumPlayerDistance * settings.minimumPlayerDistance;

        std::vector<SpawnPoint> candidates;
        candidates.reserve(walkablePositions.size());

        for (const auto& position : walkablePositions)
        {
            const float dx = position.first - playerPosition.x;
            const float dy = position.second - playerPosition.y;
            if (dx * dx + dy * dy >= minimumDistanceSquared)
            {
                candidates.push_back(position);
            }
        }

        std::mt19937 randomEngine(std::random_device{}());
        std::shuffle(candidates.begin(), candidates.end(), randomEngine);

        unsigned int spawnedCount = 0;
        for (const auto& position : candidates)
        {
            if (spawnedCount >= settings.count)
            {
                break;
            }

            auto enemy = factory(player, position.first, position.second);
            if (enemy == nullptr)
            {
                LOG_WARN("CreeperSpawner: factory returned a null enemy");
                continue;
            }

            enemies.AddEnemy(std::move(enemy));
            ++spawnedCount;
        }

        if (spawnedCount < settings.count)
        {
            LOG_WARN("CreeperSpawner: not enough safe walkable spawn points");
        }

        LOG_INFO("CreeperSpawner: spawned " + std::to_string(spawnedCount) + " creepers");
        return spawnedCount;
    }
} // namespace XYZRoguelike
