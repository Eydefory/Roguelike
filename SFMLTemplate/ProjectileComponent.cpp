#include "ProjectileComponent.h"

#include "CameraComponent.h"
#include "Constants.h"
#include "Enemy.h"
#include "EnemyContainer.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "InventoryHudComponent.h"
#include "PhysicsSystem.h"
#include "SpriteColliderComponent.h"
#include "TransformComponent.h"

#include <algorithm>
#include <cmath>
#include <string>

namespace XYZRoguelike
{
    ProjectileComponent::ProjectileComponent(XYZEngine::GameObject* gameObject,
                                             XYZEngine::GameObject* player, EnemyContainer* enemies,
                                             float directionX, float directionY)
        : Component(gameObject), player(player), enemies(enemies), directionX(directionX),
          directionY(directionY)
    {
        const float length = std::sqrt(directionX * directionX + directionY * directionY);
        if (length > 0.f)
        {
            this->directionX /= length;
            this->directionY /= length;
        }
    }

    void ProjectileComponent::Update(float deltaTime)
    {
        if (enemies == nullptr || !std::isfinite(deltaTime) || deltaTime <= 0.f)
        {
            return;
        }

        auto* transform = gameObject->GetComponent<XYZEngine::TransformComponent>();
        auto* collider = gameObject->GetComponent<XYZEngine::SpriteColliderComponent>();
        if (transform == nullptr || collider == nullptr)
        {
            XYZEngine::GameWorld::Instance()->DestroyGameObject(gameObject);
            return;
        }

        const float step = PLAYER_PROJECTILE_SPEED * deltaTime;
        const auto previousPosition = transform->GetWorldPosition();
        XYZEngine::PhysicsSystem::Instance()->MoveCollider(collider, directionX * step,
                                                           directionY * step);
        const auto currentPosition = transform->GetWorldPosition();
        const float movedX = currentPosition.x - previousPosition.x;
        const float movedY = currentPosition.y - previousPosition.y;
        const float actualStep = std::sqrt(movedX * movedX + movedY * movedY);
        distanceTravelled += actualStep;

        Enemy* enemy = enemies->GetEnemyHitBySegment(previousPosition.x, previousPosition.y,
                                                     currentPosition.x, currentPosition.y,
                                                     24.f, 24.f);
        if (enemy != nullptr)
        {
            auto* health = enemy->GetHealthComponent();
            if (health != nullptr && health->IsAlive())
            {
                const int previousHealth = health->GetHealth();
                const int previousArmor = health->GetArmor();
                health->TakeDamage(PLAYER_BOW_DAMAGE);
                auto* camera = player != nullptr
                                   ? player->GetComponent<XYZEngine::CameraComponent>()
                                   : nullptr;
                if (camera != nullptr)
                {
                    camera->Shake(0.18f, 8.f);
                }

                auto* hud = player != nullptr
                                ? player->GetComponent<InventoryHudComponent>()
                                : nullptr;
                if (hud != nullptr)
                {
                    hud->ShowFeedback("Arrow hit! " +
                                      std::to_string(previousHealth - health->GetHealth() +
                                                     previousArmor - health->GetArmor()) +
                                      " damage.",
                                      sf::Color(255, 190, 160));
                }

                if (!health->IsAlive())
                {
                    auto* inventory = player != nullptr
                                          ? player->GetComponent<InventoryComponent>()
                                          : nullptr;
                    const ItemType potion = inventory != nullptr
                                                ? inventory->AddRandomPotionDrop()
                                                : ItemType::Empty;
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
                    enemies->RemoveEnemy(enemy);
                }

                XYZEngine::GameWorld::Instance()->DestroyGameObject(gameObject);
                return;
            }
        }

        const float expectedX = directionX * step;
        const float expectedY = directionY * step;
        const bool blocked = std::abs(movedX - expectedX) > 0.5f ||
                             std::abs(movedY - expectedY) > 0.5f;
        if (blocked || distanceTravelled >= PLAYER_BOW_RANGE)
        {
            XYZEngine::GameWorld::Instance()->DestroyGameObject(gameObject);
        }
    }

    void ProjectileComponent::Render() {}
} // namespace XYZRoguelike
