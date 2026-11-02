#include "Enemy.h"
#include "EnemyAttackComponent.h"
#include "Constants.h"
#include "RigidbodyComponent.h"
#include "Loger.h"
#include <cmath>
#include <stdexcept>

namespace XYZRoguelike
{
    Enemy::Enemy(XYZEngine::GameObject* player)
        : gameObject(nullptr),
        movementSpeed(ENEMY_SPEED),
        stopDistance(ENEMY_STOP_DISTANCE)
    {
        if (player == nullptr)
        {
            LOG_ERROR("Enemy creation failed: player is null");
            throw std::invalid_argument("Enemy requires player");
        }

        gameObject = XYZEngine::GameWorld::Instance()
            ->CreateGameObject("Enemy");

        if (gameObject == nullptr)
        {
            LOG_ERROR("Enemy GameObject creation failed");
            throw std::runtime_error("Failed to create Enemy GameObject");
        }

        auto renderer = gameObject->AddComponent
            <XYZEngine::SpriteRendererComponent>();

        const sf::Texture* texture =
            XYZEngine::ResourceSystem::Instance()
            ->GetTextureShared("enemy");

        if (texture != nullptr)
        {
            renderer->SetTexture(*texture);
            renderer->SetPixelSize(ENEMY_WIDTH, ENEMY_HEIGHT);
        }

        gameObject->AddComponent
            <XYZEngine::RigidbodyComponent>();

        gameObject->AddComponent
            <XYZEngine::SpriteColliderComponent>();

        auto search =
            gameObject->AddComponent<EnemySearchComponent>();

        search->SetTarget(player);

        auto health =
            gameObject->AddComponent<HealthComponent>();

        health->Initialize(60, 10);

        auto attack =
            gameObject->AddComponent<EnemyAttackComponent>();

        attack->SetTarget(player);

        gameObject->GetComponent
            <XYZEngine::TransformComponent>()
            ->SetWorldPosition(150.f, 150.f);

        LOG_INFO("Enemy created");
    }

    XYZEngine::GameObject* Enemy::GetGameObject()
    {
        return gameObject;
    }

    HealthComponent* Enemy::GetHealthComponent()
    {
        return gameObject->GetComponent<HealthComponent>();
    }

    void Enemy::Update(float deltaTime)
    {
        auto health = GetHealthComponent();

        if (health == nullptr || !health->IsAlive())
            return;

        auto search =
            gameObject->GetComponent<EnemySearchComponent>();

        if (search == nullptr || !search->IsTargetDetected())
            return;

        auto target = search->GetTarget();

        if (target == nullptr)
            return;

        auto enemyTransform = gameObject->GetComponent
            <XYZEngine::TransformComponent>();

        auto playerTransform = target->GetComponent
            <XYZEngine::TransformComponent>();

        if (enemyTransform == nullptr ||
            playerTransform == nullptr)
            return;

        auto enemyPosition = enemyTransform->GetWorldPosition();
        auto playerPosition = playerTransform->GetWorldPosition();

        float dx = playerPosition.x - enemyPosition.x;
        float dy = playerPosition.y - enemyPosition.y;

        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= stopDistance || distance == 0.f)
            return;

        float directionX = dx / distance;
        float directionY = dy / distance;

        enemyTransform->MoveBy(
            directionX * movementSpeed * deltaTime,
            directionY * movementSpeed * deltaTime
        );
    }
}