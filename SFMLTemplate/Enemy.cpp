#include "Enemy.h"

#include "Constants.h"
#include "EnemyAttackComponent.h"
#include "EnemySearchComponent.h"
#include "Loger.h"
#include "ResourceSystem.h"
#include "RigidbodyComponent.h"
#include "SpriteColliderComponent.h"
#include "SpriteRendererComponent.h"
#include "TransformComponent.h"

#include <cmath>
#include <stdexcept>

namespace XYZRoguelike
{
    Enemy::Enemy(XYZEngine::GameObject* player, const EnemyConfig& config, float worldX,
                 float worldY)
    {
        if (player == nullptr)
        {
            LOG_ERROR("Enemy creation failed: player is null");
            throw std::invalid_argument("Enemy requires a player");
        }

        if (config.objectName.empty() || config.maxHealth <= 0 || config.armor < 0 ||
            config.attackDamage <= 0 || config.movementSpeed < 0.f ||
            config.detectionRadius <= 0.f || config.stopDistance < 0.f ||
            config.attackCooldown <= 0.f || config.attackDistance < 0.f ||
            !std::isfinite(config.movementSpeed) || !std::isfinite(config.detectionRadius) ||
            !std::isfinite(config.stopDistance) || !std::isfinite(config.attackCooldown) ||
            !std::isfinite(config.attackDistance))
        {
            throw std::invalid_argument("Enemy configuration is invalid");
        }

        gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject(config.objectName);

        if (gameObject == nullptr)
        {
            LOG_ERROR("Enemy GameObject creation failed");
            throw std::runtime_error("Failed to create Enemy GameObject");
        }

        auto* renderer = gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();
        const sf::Texture* texture =
            XYZEngine::ResourceSystem::Instance()->GetTextureShared(config.textureName);

        if (texture != nullptr)
        {
            renderer->SetTexture(*texture);
            renderer->SetPixelSize(ENEMY_WIDTH, ENEMY_HEIGHT);
        }

        gameObject->AddComponent<XYZEngine::RigidbodyComponent>();
        gameObject->AddComponent<XYZEngine::SpriteColliderComponent>();

        auto* search = gameObject->AddComponent<EnemySearchComponent>();
        search->SetTarget(player);
        search->SetMovementSettings(config.detectionRadius, config.movementSpeed,
                                    config.stopDistance);

        auto* health = gameObject->AddComponent<HealthComponent>();
        health->Initialize(config.maxHealth, config.armor);

        auto* attack = gameObject->AddComponent<EnemyAttackComponent>();
        attack->SetTarget(player);
        attack->SetAttackSettings(config.attackDamage, config.attackCooldown,
                                  config.attackDistance);

        gameObject->GetComponent<XYZEngine::TransformComponent>()->SetWorldPosition(worldX, worldY);

        LOG_INFO(config.objectName + " created");
    }

    XYZEngine::GameObject* Enemy::GetGameObject() const
    {
        return gameObject;
    }

    HealthComponent* Enemy::GetHealthComponent() const
    {
        return gameObject != nullptr ? gameObject->GetComponent<HealthComponent>() : nullptr;
    }
} // namespace XYZRoguelike
