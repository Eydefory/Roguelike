#include "EnemySearchComponent.h"
#include "Constants.h"

#include <cmath>
#include <algorithm>

namespace XYZRoguelike
{
    EnemySearchComponent::EnemySearchComponent(XYZEngine::GameObject* gameObject)
        : Component(gameObject),
        target(nullptr),
        detectionRadius(ENEMY_DETECTION_RADIUS),
        movementSpeed(ENEMY_SPEED),
        stopDistance(ENEMY_STOP_DISTANCE),
        targetDetected(false)
    {
    }

    void EnemySearchComponent::Update(float deltaTime)
    {
        if (target == nullptr)
        {
            targetDetected = false;
            return;
        }

        auto enemyTransform =
            GetGameObject()->GetComponent<XYZEngine::TransformComponent>();

        auto targetTransform =
            target->GetComponent<XYZEngine::TransformComponent>();

        if (enemyTransform == nullptr || targetTransform == nullptr)
            return;

        auto enemyPosition = enemyTransform->GetWorldPosition();
        auto targetPosition = targetTransform->GetWorldPosition();

        float dx = targetPosition.x - enemyPosition.x;
        float dy = targetPosition.y - enemyPosition.y;

        float distance = std::sqrt(dx * dx + dy * dy);

        targetDetected = distance <= detectionRadius;

        if (!targetDetected || distance <= stopDistance || distance == 0.f)
            return;

        float directionX = dx / distance;
        float directionY = dy / distance;

        float moveX = directionX * movementSpeed * deltaTime;
        float moveY = directionY * movementSpeed * deltaTime;

        enemyTransform->MoveBy(moveX, moveY);

        
        auto position = enemyTransform->GetWorldPosition();

        constexpr float halfEnemy = 24.f;

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

    void EnemySearchComponent::Render()
    {
    }

    void EnemySearchComponent::SetTarget(XYZEngine::GameObject* newTarget)
    {
        target = newTarget;
    }

    bool EnemySearchComponent::IsTargetDetected() const
    {
        return targetDetected;
    }

    XYZEngine::GameObject* EnemySearchComponent::GetTarget() const
    {
        return target;
    }
}