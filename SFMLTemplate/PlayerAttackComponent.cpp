#include "PlayerAttackComponent.h"
#include "EnemyContainer.h"
#include "Enemy.h"
#include "HealthComponent.h"
#include "Loger.h"

#include <SFML/Window/Keyboard.hpp>

namespace XYZRoguelike
{
    PlayerAttackComponent::PlayerAttackComponent(
        XYZEngine::GameObject* gameObject)
        : Component(gameObject),
        enemyContainer(nullptr),
        attackCooldown(0.5f),
        attackTimer(0.f)
    {
    }

    void PlayerAttackComponent::SetEnemyContainer(
        EnemyContainer* container)
    {
        enemyContainer = container;
    }

    void PlayerAttackComponent::Update(float deltaTime)
    {
        attackTimer += deltaTime;

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            return;

        if (attackTimer < attackCooldown)
            return;

        if (enemyContainer == nullptr)
            return;

        Enemy* enemy =
            enemyContainer->GetNearestEnemy(gameObject);

        if (enemy == nullptr)
            return;

        HealthComponent* health =
            enemy->GetHealthComponent();

        if (health == nullptr || !health->IsAlive())
            return;

        attackTimer = 0.f;

        health->TakeDamage(20);

        LOG_INFO("Player attacked enemy");
    }

    void PlayerAttackComponent::Render()
    {
    }
}