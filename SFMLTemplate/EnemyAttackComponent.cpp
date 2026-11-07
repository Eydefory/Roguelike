#include "EnemyAttackComponent.h"
#include "HealthComponent.h"
#include "InventoryHudComponent.h"
#include "Loger.h"
#include "TransformComponent.h"

#include <cmath>
#include <stdexcept>

namespace XYZRoguelike
{
    EnemyAttackComponent::EnemyAttackComponent(XYZEngine::GameObject* gameObject)
        : Component(gameObject), target(nullptr), attackCooldown(1.f), attackTimer(0.f),
          attackDistance(55.f), attackDamage(10)
    {
    }

    void EnemyAttackComponent::SetTarget(XYZEngine::GameObject* newTarget)
    {
        target = newTarget;
    }

    void EnemyAttackComponent::SetAttackSettings(int newDamage, float newCooldown,
                                                 float newAttackDistance)
    {
        if (newDamage <= 0 || newCooldown <= 0.f || newAttackDistance < 0.f ||
            !std::isfinite(newCooldown) || !std::isfinite(newAttackDistance))
        {
            throw std::invalid_argument("Enemy attack settings are invalid");
        }

        attackDamage = newDamage;
        attackCooldown = newCooldown;
        attackDistance = newAttackDistance;
    }

    void EnemyAttackComponent::Update(float deltaTime)
    {
        if (target == nullptr)
            return;

        auto* inventoryHud = target->GetComponent<InventoryHudComponent>();
        if (inventoryHud != nullptr && inventoryHud->IsOpen())
            return;

        attackTimer += deltaTime;

        auto enemyHealth = gameObject->GetComponent<HealthComponent>();

        auto playerHealth = target->GetComponent<HealthComponent>();

        if (enemyHealth == nullptr || playerHealth == nullptr || !enemyHealth->IsAlive() ||
            !playerHealth->IsAlive())
            return;

        auto enemyTransform = gameObject->GetComponent<XYZEngine::TransformComponent>();

        auto playerTransform = target->GetComponent<XYZEngine::TransformComponent>();

        if (enemyTransform == nullptr || playerTransform == nullptr)
            return;

        auto enemyPosition = enemyTransform->GetWorldPosition();

        auto playerPosition = playerTransform->GetWorldPosition();

        float dx = playerPosition.x - enemyPosition.x;
        float dy = playerPosition.y - enemyPosition.y;

        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > attackDistance)
            return;

        if (attackTimer < attackCooldown)
            return;

        attackTimer = 0.f;

        playerHealth->TakeHit(attackDamage);

        LOG_INFO("Enemy attacked player");
    }

    void EnemyAttackComponent::Render() {}
} // namespace XYZRoguelike
