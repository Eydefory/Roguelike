#include "EnemyContainer.h"
#include "HealthComponent.h"
#include "TransformComponent.h"
#include "Loger.h"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace XYZRoguelike
{
    EnemyContainer::EnemyContainer(
        XYZEngine::GameObject* target)
    {
        CreateEnemies(target);
    }

    void EnemyContainer::CreateEnemies(
        XYZEngine::GameObject* target)
    {
        enemies.clear();

        if (target == nullptr)
        {
            LOG_ERROR("EnemyContainer target is null");
            throw std::invalid_argument("Enemy target is null");
        }

        enemies.push_back(std::make_unique<Enemy>(target));

        LOG_INFO("EnemyContainer initialized");
    }

    Enemy* EnemyContainer::GetNearestEnemy(
        XYZEngine::GameObject* target)
    {
        if (target == nullptr)
            return nullptr;

        auto targetTransform =
            target->GetComponent
            <XYZEngine::TransformComponent>();

        if (targetTransform == nullptr)
            return nullptr;

        auto targetPosition =
            targetTransform->GetWorldPosition();

        Enemy* nearestEnemy = nullptr;

        float nearestDistance =
            std::numeric_limits<float>::max();

        for (auto& enemy : enemies)
        {
            if (enemy == nullptr)
                continue;

            auto health = enemy->GetHealthComponent();

            if (health == nullptr || !health->IsAlive())
                continue;

            auto transform =
                enemy->GetGameObject()->GetComponent
                <XYZEngine::TransformComponent>();

            if (transform == nullptr)
                continue;

            auto position = transform->GetWorldPosition();

            float dx = position.x - targetPosition.x;
            float dy = position.y - targetPosition.y;

            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < nearestDistance)
            {
                nearestDistance = distance;
                nearestEnemy = enemy.get();
            }
        }

        return nearestEnemy;
    }
}