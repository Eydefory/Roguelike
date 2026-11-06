#pragma once

#include "Enemy.h"

#include <cstddef>
#include <memory>
#include <vector>

namespace XYZRoguelike
{
    class EnemyContainer
    {
    public:
        Enemy* AddEnemy(std::unique_ptr<Enemy> enemy);
        bool RemoveEnemy(Enemy* enemy);

        Enemy* GetNearestEnemy(XYZEngine::GameObject* target) const;
        std::size_t GetEnemyCount() const;

    private:
        std::vector<std::unique_ptr<Enemy>> enemies;
    };
} // namespace XYZRoguelike
