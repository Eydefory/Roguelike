#include "EnemyContainer.h"

namespace XYZRoguelike
{
    EnemyContainer::EnemyContainer(XYZEngine::GameObject* target)
    {
        CreateEnemies(target);
    }

    void EnemyContainer::CreateEnemies(XYZEngine::GameObject* target)
    {
        enemies.clear();

        if (target == nullptr)
        {
            return;
        }

        enemies.push_back(std::make_unique<Enemy>(target));
    }
}