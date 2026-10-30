#pragma once

#include "Enemy.h"
#include <memory>
#include <vector>

namespace XYZRoguelike
{
    class EnemyContainer
    {
    public:
        explicit EnemyContainer(XYZEngine::GameObject* target);

        void CreateEnemies(XYZEngine::GameObject* target);

    private:
        std::vector<std::unique_ptr<Enemy>> enemies;
    };
}