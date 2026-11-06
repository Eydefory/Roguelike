#include "EnemyContainer.h"

#include "GameWorld.h"
#include "Loger.h"
#include "TransformComponent.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <utility>

namespace XYZRoguelike
{
    Enemy* EnemyContainer::AddEnemy(std::unique_ptr<Enemy> enemy)
    {
        if (enemy == nullptr || enemy->GetGameObject() == nullptr)
        {
            LOG_ERROR("Cannot add an invalid enemy to EnemyContainer");
            throw std::invalid_argument("Enemy cannot be null");
        }

        Enemy* result = enemy.get();
        enemies.push_back(std::move(enemy));
        LOG_INFO("EnemyContainer: enemy added");
        return result;
    }

    bool EnemyContainer::RemoveEnemy(Enemy* enemy)
    {
        if (enemy == nullptr)
        {
            return false;
        }

        auto it = std::find_if(enemies.begin(), enemies.end(),
                               [enemy](const std::unique_ptr<Enemy>& candidate)
                               { return candidate.get() == enemy; });

        if (it == enemies.end())
        {
            return false;
        }

        auto* gameObject = (*it)->GetGameObject();
        if (gameObject != nullptr)
        {
            XYZEngine::GameWorld::Instance()->DestroyGameObject(gameObject);
        }

        enemies.erase(it);
        LOG_INFO("EnemyContainer: enemy removed");
        return true;
    }

    Enemy* EnemyContainer::GetNearestEnemy(XYZEngine::GameObject* target) const
    {
        if (target == nullptr)
        {
            return nullptr;
        }

        auto* targetTransform = target->GetComponent<XYZEngine::TransformComponent>();
        if (targetTransform == nullptr)
        {
            return nullptr;
        }

        const auto targetPosition = targetTransform->GetWorldPosition();
        Enemy* nearestEnemy = nullptr;
        float nearestDistance = std::numeric_limits<float>::max();

        for (const auto& enemy : enemies)
        {
            if (enemy == nullptr || enemy->GetGameObject() == nullptr)
            {
                continue;
            }

            auto* health = enemy->GetHealthComponent();
            if (health == nullptr || !health->IsAlive())
            {
                continue;
            }

            auto* transform = enemy->GetGameObject()->GetComponent<XYZEngine::TransformComponent>();
            if (transform == nullptr)
            {
                continue;
            }

            const auto position = transform->GetWorldPosition();
            const float dx = position.x - targetPosition.x;
            const float dy = position.y - targetPosition.y;
            const float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < nearestDistance)
            {
                nearestDistance = distance;
                nearestEnemy = enemy.get();
            }
        }

        return nearestEnemy;
    }

    std::size_t EnemyContainer::GetEnemyCount() const
    {
        return enemies.size();
    }
} // namespace XYZRoguelike
