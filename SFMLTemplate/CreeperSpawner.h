#pragma once

#include "Constants.h"
#include "Enemy.h"

#include <functional>
#include <memory>
#include <utility>
#include <vector>

namespace XYZRoguelike
{
    class EnemyContainer;

    struct CreeperSpawnSettings
    {
        unsigned int count = CREEPER_SPAWN_COUNT;
        float minimumPlayerDistance = CREEPER_MIN_PLAYER_DISTANCE;
    };

    class CreeperSpawner
    {
    public:
        using SpawnPoint = std::pair<float, float>;
        // A factory makes the spawn algorithm reusable for other enemy types.
        using EnemyFactory =
            std::function<std::unique_ptr<Enemy>(XYZEngine::GameObject*, float, float)>;

        explicit CreeperSpawner(CreeperSpawnSettings settings = CreeperSpawnSettings{},
                                EnemyFactory factory = {});

        unsigned int SpawnCreepers(EnemyContainer& enemies, XYZEngine::GameObject* player,
                                   const std::vector<SpawnPoint>& walkablePositions) const;

    private:
        CreeperSpawnSettings settings;
        EnemyFactory factory;
    };
} // namespace XYZRoguelike
