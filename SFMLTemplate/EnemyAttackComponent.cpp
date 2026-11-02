#include "EnemyAttackComponent.h"
#include "HealthComponent.h"
#include "TransformComponent.h"
#include "Loger.h"

#include <cmath>

namespace XYZRoguelike
{
    EnemyAttackComponent::EnemyAttackComponent(
        XYZEngine::GameObject* gameObject)
        : Component(gameObject),
        target(nullptr),
        attackCooldown(1.f),
        attackTimer(0.f),
        attackDistance(55.f)
    {
    }

    void EnemyAttackComponent::SetTarget(
        XYZEngine::GameObject* newTarget)
    {
        target = newTarget;
    }

    void EnemyAttackComponent::Update(float deltaTime)
    {
        attackTimer += deltaTime;

        if (target == nullptr)
            return;

        auto enemyHealth =
            gameObject->GetComponent<HealthComponent>();

        auto playerHealth =
            target->GetComponent<HealthComponent>();

        if (enemyHealth == nullptr ||
            playerHealth == nullptr ||
            !enemyHealth->IsAlive() ||
            !playerHealth->IsAlive())
            return;

        auto enemyTransform =
            gameObject->GetComponent
            <XYZEngine::TransformComponent>();

        auto playerTransform =
            target->GetComponent
            <XYZEngine::TransformComponent>();

        if (enemyTransform == nullptr ||
            playerTransform == nullptr)
            return;

        auto enemyPosition =
            enemyTransform->GetWorldPosition();

        auto playerPosition =
            playerTransform->GetWorldPosition();

        float dx = playerPosition.x - enemyPosition.x;
        float dy = playerPosition.y - enemyPosition.y;

        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > attackDistance)
            return;

        if (attackTimer < attackCooldown)
            return;

        attackTimer = 0.f;

        playerHealth->TakeDamage(10);

        LOG_INFO("Enemy attacked player");
    }

    void EnemyAttackComponent::Render()
    {
    }
}