#pragma once

#include "Enemy.h"

#include <cstddef>
#include <functional>
#include <memory>
#include <vector>

namespace XYZRoguelike
{
    class EnemyContainer
    {
    public:
        Enemy* AddEnemy(std::unique_ptr<Enemy> enemy);
        bool RemoveEnemy(Enemy* enemy);
        void SetAllEnemiesDefeatedCallback(std::function<void(float, float)> callback);
        void SetNavigationGrid(const std::vector<std::vector<bool>>& grid);

        Enemy* GetNearestEnemy(XYZEngine::GameObject* target) const;
        Enemy* GetEnemyHitBySegment(float startX, float startY, float endX, float endY,
                                    float projectileHalfWidth, float projectileHalfHeight) const;
        bool IsPositionNearEnemy(float worldX, float worldY, float radius) const;
        std::size_t GetEnemyCount() const;
        std::size_t GetTotalEnemyCount() const;

    private:
        std::vector<std::unique_ptr<Enemy>> enemies;
        std::vector<std::vector<bool>> navigationGrid;
        std::size_t totalEnemyCount = 0;
        std::function<void(float, float)> onAllEnemiesDefeated;
        bool allEnemiesDefeatedNotified = false;
    };
} // namespace XYZRoguelike
