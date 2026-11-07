#include "EnemySearchComponent.h"
#include "Constants.h"
#include "InventoryHudComponent.h"
#include "PhysicsSystem.h"
#include "SpriteColliderComponent.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <queue>
#include <stdexcept>

namespace XYZRoguelike
{
    EnemySearchComponent::EnemySearchComponent(XYZEngine::GameObject* gameObject)
        : Component(gameObject), target(nullptr), detectionRadius(ENEMY_DETECTION_RADIUS),
          movementSpeed(ENEMY_SPEED), stopDistance(ENEMY_STOP_DISTANCE), targetDetected(false)
    {
    }

    void EnemySearchComponent::Update(float deltaTime)
    {
        if (target == nullptr)
        {
            targetDetected = false;
            return;
        }

        auto enemyTransform = GetGameObject()->GetComponent<XYZEngine::TransformComponent>();

        auto targetTransform = target->GetComponent<XYZEngine::TransformComponent>();

        if (enemyTransform == nullptr || targetTransform == nullptr)
            return;

        auto* inventoryHud = target->GetComponent<InventoryHudComponent>();
        if (inventoryHud != nullptr && inventoryHud->IsOpen())
        {
            targetDetected = false;
            return;
        }

        auto enemyPosition = enemyTransform->GetWorldPosition();
        auto targetPosition = targetTransform->GetWorldPosition();

        const float targetDx = targetPosition.x - enemyPosition.x;
        const float targetDy = targetPosition.y - enemyPosition.y;
        const float targetDistance = std::sqrt(targetDx * targetDx + targetDy * targetDy);

        targetDetected = targetDistance <= detectionRadius;

        if (!targetDetected || targetDistance <= stopDistance)
            return;

        sf::Vector2f waypoint(targetPosition.x, targetPosition.y);
        if (!navigationGrid.empty() && !navigationGrid.front().empty())
        {
            const int rows = static_cast<int>(navigationGrid.size());
            const int columns = static_cast<int>(navigationGrid.front().size());
            const auto toCellX = [](float worldX)
            {
                return static_cast<int>(std::floor((worldX - ROOM_OFFSET_X) / TILE_SIZE));
            };
            const auto toCellY = [](float worldY)
            {
                return static_cast<int>(std::floor((worldY - ROOM_OFFSET_Y) / TILE_SIZE));
            };
            const int startX = std::clamp(toCellX(enemyPosition.x), 0, columns - 1);
            const int startY = std::clamp(toCellY(enemyPosition.y), 0, rows - 1);
            const int goalX = std::clamp(toCellX(targetPosition.x), 0, columns - 1);
            const int goalY = std::clamp(toCellY(targetPosition.y), 0, rows - 1);
            const int start = startY * columns + startX;
            const int goal = goalY * columns + goalX;
            const auto isWall = [this, columns](int index)
            {
                return navigationGrid[static_cast<std::size_t>(index / columns)]
                                     [static_cast<std::size_t>(index % columns)];
            };

            if (isWall(start) || isWall(goal))
            {
                return;
            }

            std::vector<int> parent(static_cast<std::size_t>(rows * columns), -1);
            std::queue<int> open;
            parent[static_cast<std::size_t>(start)] = start;
            open.push(start);
            const std::array<sf::Vector2i, 4> DIRECTIONS = {
                sf::Vector2i(1, 0), sf::Vector2i(-1, 0), sf::Vector2i(0, 1),
                sf::Vector2i(0, -1)};

            while (!open.empty() && parent[static_cast<std::size_t>(goal)] == -1)
            {
                const int current = open.front();
                open.pop();
                const int cellX = current % columns;
                const int cellY = current / columns;
                for (const auto& direction : DIRECTIONS)
                {
                    const int nextX = cellX + direction.x;
                    const int nextY = cellY + direction.y;
                    if (nextX < 0 || nextX >= columns || nextY < 0 || nextY >= rows)
                    {
                        continue;
                    }
                    const int next = nextY * columns + nextX;
                    if (isWall(next) || parent[static_cast<std::size_t>(next)] != -1)
                    {
                        continue;
                    }
                    parent[static_cast<std::size_t>(next)] = current;
                    open.push(next);
                }
            }

            if (parent[static_cast<std::size_t>(goal)] == -1)
            {
                return;
            }

            int next = goal;
            while (parent[static_cast<std::size_t>(next)] != start && next != start)
            {
                next = parent[static_cast<std::size_t>(next)];
            }
            if (next != start)
            {
                const int nextCellX = next % columns;
                const int nextCellY = next / columns;
                waypoint = sf::Vector2f(ROOM_OFFSET_X + (nextCellX + 0.5f) * TILE_SIZE,
                                        ROOM_OFFSET_Y + (nextCellY + 0.5f) * TILE_SIZE);
            }
        }

        const float dx = waypoint.x - enemyPosition.x;
        const float dy = waypoint.y - enemyPosition.y;
        const float distance = std::sqrt(dx * dx + dy * dy);
        if (distance <= 0.001f)
        {
            return;
        }

        const float directionX = dx / distance;
        const float directionY = dy / distance;

        float moveX = directionX * movementSpeed * deltaTime;
        float moveY = directionY * movementSpeed * deltaTime;

        auto collider = GetGameObject()->GetComponent<XYZEngine::SpriteColliderComponent>();
        if (collider != nullptr)
        {
            XYZEngine::PhysicsSystem::Instance()->MoveCollider(collider, moveX, moveY);
        }
        else
        {
            enemyTransform->MoveBy(moveX, moveY);
        }

        auto position = enemyTransform->GetWorldPosition();

        float minX = ROOM_MIN_ENEMY_X;
        float maxX = ROOM_MAX_ENEMY_X;

        float minY = ROOM_MIN_ENEMY_Y;
        float maxY = ROOM_MAX_ENEMY_Y;

        float clampedX = std::max(minX, std::min(position.x, maxX));
        float clampedY = std::max(minY, std::min(position.y, maxY));

        if (position.x != clampedX || position.y != clampedY)
        {
            enemyTransform->SetWorldPosition(clampedX, clampedY);
        }
    }

    void EnemySearchComponent::Render() {}

    void EnemySearchComponent::SetTarget(XYZEngine::GameObject* newTarget)
    {
        target = newTarget;
    }

    void EnemySearchComponent::SetMovementSettings(float newDetectionRadius, float newMovementSpeed,
                                                   float newStopDistance)
    {
        if (newDetectionRadius <= 0.f || newMovementSpeed < 0.f || newStopDistance < 0.f ||
            !std::isfinite(newDetectionRadius) || !std::isfinite(newMovementSpeed) ||
            !std::isfinite(newStopDistance))
        {
            throw std::invalid_argument("Enemy movement settings are invalid");
        }

        detectionRadius = newDetectionRadius;
        movementSpeed = newMovementSpeed;
        stopDistance = newStopDistance;
    }

    void EnemySearchComponent::SetNavigationGrid(const std::vector<std::vector<bool>>& grid)
    {
        navigationGrid = grid;
    }

    bool EnemySearchComponent::IsTargetDetected() const
    {
        return targetDetected;
    }

    XYZEngine::GameObject* EnemySearchComponent::GetTarget() const
    {
        return target;
    }
} // namespace XYZRoguelike
