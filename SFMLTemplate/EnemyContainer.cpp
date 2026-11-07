#include "EnemyContainer.h"

#include "Constants.h"
#include "GameWorld.h"
#include "Loger.h"
#include "EnemySearchComponent.h"
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
        if (!navigationGrid.empty() && result->GetGameObject() != nullptr)
        {
            if (auto* search = result->GetGameObject()->GetComponent<EnemySearchComponent>())
            {
                search->SetNavigationGrid(navigationGrid);
            }
        }
        ++totalEnemyCount;
        allEnemiesDefeatedNotified = false;
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
        float deathX = 0.f;
        float deathY = 0.f;
        if (gameObject != nullptr)
        {
            auto* transform = gameObject->GetComponent<XYZEngine::TransformComponent>();
            if (transform != nullptr)
            {
                const auto position = transform->GetWorldPosition();
                deathX = position.x;
                deathY = position.y;
            }
            XYZEngine::GameWorld::Instance()->DestroyGameObject(gameObject);
        }

        enemies.erase(it);
        LOG_INFO("EnemyContainer: enemy removed");

        if (enemies.empty() && !allEnemiesDefeatedNotified)
        {
            allEnemiesDefeatedNotified = true;
            if (onAllEnemiesDefeated)
            {
                onAllEnemiesDefeated(deathX, deathY);
            }
        }

        return true;
    }

    void EnemyContainer::SetAllEnemiesDefeatedCallback(std::function<void(float, float)> callback)
    {
        onAllEnemiesDefeated = std::move(callback);
    }

    void EnemyContainer::SetNavigationGrid(const std::vector<std::vector<bool>>& grid)
    {
        navigationGrid = grid;
        for (const auto& enemy : enemies)
        {
            if (enemy == nullptr || enemy->GetGameObject() == nullptr)
            {
                continue;
            }
            if (auto* search = enemy->GetGameObject()->GetComponent<EnemySearchComponent>())
            {
                search->SetNavigationGrid(navigationGrid);
            }
        }
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

    Enemy* EnemyContainer::GetEnemyHitBySegment(float startX, float startY, float endX,
                                                float endY, float projectileHalfWidth,
                                                float projectileHalfHeight) const
    {
        const float segmentX = endX - startX;
        const float segmentY = endY - startY;
        const float targetHalfWidth = ENEMY_WIDTH * 0.5f + projectileHalfWidth;
        const float targetHalfHeight = ENEMY_HEIGHT * 0.5f + projectileHalfHeight;
        float nearestAlongSegment = 1.f;
        Enemy* hitEnemy = nullptr;

        for (const auto& enemy : enemies)
        {
            if (enemy == nullptr || enemy->GetGameObject() == nullptr)
            {
                continue;
            }

            auto* health = enemy->GetHealthComponent();
            auto* transform = enemy->GetGameObject()->GetComponent<XYZEngine::TransformComponent>();
            if (health == nullptr || !health->IsAlive() || transform == nullptr)
            {
                continue;
            }

            const auto position = transform->GetWorldPosition();
            float entry = 0.f;
            float exit = 1.f;
            const auto clipAxis = [&entry, &exit](float origin, float delta, float minimum,
                                                  float maximum)
            {
                if (std::abs(delta) < 0.0001f)
                {
                    return origin >= minimum && origin <= maximum;
                }

                float first = (minimum - origin) / delta;
                float second = (maximum - origin) / delta;
                if (first > second)
                {
                    std::swap(first, second);
                }
                entry = std::max(entry, first);
                exit = std::min(exit, second);
                return entry <= exit;
            };

            const bool intersects =
                clipAxis(startX, segmentX, position.x - targetHalfWidth,
                         position.x + targetHalfWidth) &&
                clipAxis(startY, segmentY, position.y - targetHalfHeight,
                         position.y + targetHalfHeight) &&
                exit >= 0.f && entry <= 1.f;
            if (intersects && entry < nearestAlongSegment)
            {
                nearestAlongSegment = entry;
                hitEnemy = enemy.get();
            }
        }

        return hitEnemy;
    }

    std::size_t EnemyContainer::GetEnemyCount() const
    {
        return enemies.size();
    }

    std::size_t EnemyContainer::GetTotalEnemyCount() const
    {
        return totalEnemyCount;
    }

    bool EnemyContainer::IsPositionNearEnemy(float worldX, float worldY, float radius) const
    {
        const float radiusSquared = radius * radius;
        for (const auto& enemy : enemies)
        {
            if (enemy == nullptr || enemy->GetGameObject() == nullptr)
            {
                continue;
            }

            auto* transform = enemy->GetGameObject()->GetComponent<XYZEngine::TransformComponent>();
            if (transform == nullptr)
            {
                continue;
            }

            const auto position = transform->GetWorldPosition();
            const float dx = position.x - worldX;
            const float dy = position.y - worldY;
            if (dx * dx + dy * dy < radiusSquared)
            {
                return true;
            }
        }

        return false;
    }
} // namespace XYZRoguelike
