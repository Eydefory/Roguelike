#include "PlayerAttackComponent.h"

#include "CameraComponent.h"
#include "Constants.h"
#include "Enemy.h"
#include "EnemyContainer.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "InventoryHudComponent.h"
#include "Loger.h"
#include "PlayerAnimationComponent.h"
#include "ProjectileComponent.h"
#include "ResourceSystem.h"
#include "RigidbodyComponent.h"
#include "SpriteColliderComponent.h"
#include "SpriteRendererComponent.h"
#include "TransformComponent.h"

#include <SFML/Window/Keyboard.hpp>

#include <cmath>
#include <string>

namespace XYZRoguelike
{
    PlayerAttackComponent::PlayerAttackComponent(XYZEngine::GameObject* gameObject)
        : Component(gameObject), enemyContainer(nullptr), attackCooldown(PLAYER_ATTACK_COOLDOWN),
          attackTimer(0.f)
    {
    }

    void PlayerAttackComponent::SetEnemyContainer(EnemyContainer* container)
    {
        enemyContainer = container;
    }

    void PlayerAttackComponent::Update(float deltaTime)
    {
        auto* inventoryHud = gameObject->GetComponent<InventoryHudComponent>();
        if (inventoryHud != nullptr && inventoryHud->IsOpen())
        {
            return;
        }

        if (enemyContainer != nullptr)
        {
            auto* playerTransform = gameObject->GetComponent<XYZEngine::TransformComponent>();
            auto* animation = gameObject->GetComponent<PlayerAnimationComponent>();
            Enemy* nearestEnemy = enemyContainer->GetNearestEnemy(gameObject);
            auto* enemyTransform =
                nearestEnemy != nullptr
                    ? nearestEnemy->GetGameObject()->GetComponent<XYZEngine::TransformComponent>()
                    : nullptr;
            if (playerTransform != nullptr && enemyTransform != nullptr && animation != nullptr)
            {
                const auto playerPosition = playerTransform->GetWorldPosition();
                const auto enemyPosition = enemyTransform->GetWorldPosition();
                const float dx = enemyPosition.x - playerPosition.x;
                const float dy = enemyPosition.y - playerPosition.y;
                constexpr float AUTO_FACING_RADIUS = 100.f;
                if (dx * dx + dy * dy <= AUTO_FACING_RADIUS * AUTO_FACING_RADIUS)
                {
                    animation->FaceToward(dx, dy);
                }
            }
        }

        attackTimer += deltaTime;
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || attackTimer < attackCooldown)
        {
            return;
        }

        auto* inventory = gameObject->GetComponent<InventoryComponent>();
        if (inventory == nullptr)
        {
            return;
        }

        switch (inventory->GetSelectedItem())
        {
        case ItemType::Sword:
            AttackWithSword(inventoryHud);
            break;
        case ItemType::Bow:
            FireBow(inventoryHud);
            break;
        case ItemType::HealingPotion:
        case ItemType::ShieldPotion:
        case ItemType::StaminaPotion:
            if (inventoryHud != nullptr)
            {
                inventoryHud->ShowFeedback("Press F to use the selected potion.",
                                           sf::Color(221, 219, 234));
            }
            break;
        case ItemType::Key:
            if (inventoryHud != nullptr)
            {
                inventoryHud->ShowFeedback("Use E near a door while holding the key.",
                                           sf::Color(221, 219, 234));
            }
            break;
        case ItemType::Empty:
        default:
            if (inventoryHud != nullptr)
            {
                inventoryHud->ShowFeedback("Select a weapon in the inventory.",
                                           sf::Color(255, 206, 135));
            }
            break;
        }
    }

    void PlayerAttackComponent::AttackWithSword(InventoryHudComponent* hud)
    {
        if (enemyContainer == nullptr)
        {
            return;
        }

        Enemy* enemy = enemyContainer->GetNearestEnemy(gameObject);
        if (enemy == nullptr)
        {
            return;
        }

        auto* playerTransform = gameObject->GetComponent<XYZEngine::TransformComponent>();
        auto* enemyTransform =
            enemy->GetGameObject()->GetComponent<XYZEngine::TransformComponent>();
        auto* health = enemy->GetHealthComponent();
        if (playerTransform == nullptr || enemyTransform == nullptr || health == nullptr ||
            !health->IsAlive())
        {
            return;
        }

        const auto playerWorldPosition = playerTransform->GetWorldPosition();
        const sf::Vector2f playerPosition(playerWorldPosition.x, playerWorldPosition.y);
        const auto enemyPosition = enemyTransform->GetWorldPosition();
        const float dx = enemyPosition.x - playerPosition.x;
        const float dy = enemyPosition.y - playerPosition.y;
        if (dx * dx + dy * dy > PLAYER_ATTACK_RANGE * PLAYER_ATTACK_RANGE)
        {
            return;
        }

        attackTimer = 0.f;
        health->TakeDamage(PLAYER_ATTACK_DAMAGE);
        if (auto* camera = gameObject->GetComponent<XYZEngine::CameraComponent>())
        {
            camera->Shake();
        }
        if (hud != nullptr)
        {
            hud->ShowFeedback("Sword hit!", sf::Color(248, 213, 196));
        }
        LOG_INFO("Player struck an enemy with the sword");

        if (!health->IsAlive())
        {
            auto* inventory = gameObject->GetComponent<InventoryComponent>();
            const ItemType potion =
                inventory != nullptr ? inventory->AddRandomPotionDrop() : ItemType::Empty;
            if (hud != nullptr)
            {
                if (potion != ItemType::Empty)
                {
                    hud->ShowFeedback(std::string(GetItemDisplayName(potion)) +
                                      " added to inventory.");
                }
                else
                {
                    hud->ShowFeedback("Inventory full. Free a slot to pick this up.",
                                      sf::Color(255, 140, 120));
                }
            }
            enemyContainer->RemoveEnemy(enemy);
        }
    }

    void PlayerAttackComponent::FireBow(InventoryHudComponent* hud)
    {
        if (enemyContainer == nullptr)
        {
            return;
        }

        auto* playerTransform = gameObject->GetComponent<XYZEngine::TransformComponent>();
        if (playerTransform == nullptr)
        {
            return;
        }

        const auto playerWorldPosition = playerTransform->GetWorldPosition();
        const sf::Vector2f playerPosition(playerWorldPosition.x, playerWorldPosition.y);
        sf::Vector2f direction(0.f, 1.f);
        if (auto* animation = gameObject->GetComponent<PlayerAnimationComponent>())
        {
            direction = animation->GetFacingDirection();
        }

        const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length <= 0.f)
        {
            return;
        }
        direction /= length;

        attackTimer = 0.f;
        XYZEngine::GameObject* player = gameObject;
        EnemyContainer* enemies = enemyContainer;
        XYZEngine::GameWorld::Instance()->ScheduleAfterUpdate(
            [player, enemies, playerPosition, direction]()
            {
                auto* projectile = XYZEngine::GameWorld::Instance()->CreateGameObject("Arrow");
                auto* renderer = projectile->AddComponent<XYZEngine::SpriteRendererComponent>();
                const sf::Texture* texture =
                    XYZEngine::ResourceSystem::Instance()->GetTextureShared("projectile");
                if (texture != nullptr)
                {
                    renderer->SetTexture(*texture);
                    renderer->SetPixelSize(32, 32);
                }

                const sf::Vector2f spawnPosition = playerPosition + direction * 46.f;
                auto* projectileTransform =
                    projectile->GetComponent<XYZEngine::TransformComponent>();
                projectileTransform->SetWorldPosition(spawnPosition.x, spawnPosition.y);
                const float directionAngle =
                    std::atan2(direction.y, direction.x) * 180.f / 3.14159265358979323846f;
                projectileTransform->SetWorldRotation(directionAngle + 45.f);
                auto* projectileCollider =
                    projectile->AddComponent<XYZEngine::SpriteColliderComponent>();
                projectileCollider->SetTrigger(true);
                projectile->AddComponent<ProjectileComponent>(player, enemies, direction.x,
                                                              direction.y);
            });

        LOG_INFO("Player fired an arrow");
    }

    void PlayerAttackComponent::Render() {}
} // namespace XYZRoguelike
