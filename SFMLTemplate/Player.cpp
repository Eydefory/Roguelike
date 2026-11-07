
#include "Player.h"
#include "Constants.h"
#include "EnemyContainer.h"
#include "HealthBarComponent.h"
#include "InventoryComponent.h"
#include "InventoryHudComponent.h"
#include "Loger.h"
#include "PlayerAnimationComponent.h"
#include "PlayerAttackComponent.h"
#include "PlayerMovementComponent.h"
#include "PotionSpawnerComponent.h"
#include "QuestHudComponent.h"
#include "RigidbodyComponent.h"
#include "StaminaComponent.h"

namespace XYZRoguelike
{
    Player::Player() : movementSpeed(PLAYER_SPEED)
    {
        gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Player");

        auto renderer = gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();

        const sf::Texture* texture =
            XYZEngine::ResourceSystem::Instance()->GetTextureShared("player");

        if (texture != nullptr)
        {
            renderer->SetTexture(*texture);
            renderer->SetPixelSize(static_cast<int>(PLAYER_WIDTH), static_cast<int>(PLAYER_HEIGHT));
        }

        gameObject->AddComponent<XYZEngine::SpriteColliderComponent>();

        gameObject->AddComponent<XYZEngine::RigidbodyComponent>()->SetKinematic(true);

        gameObject->AddComponent<XYZEngine::InputComponent>();

        auto health = gameObject->AddComponent<HealthComponent>();

        health->Initialize(PLAYER_MAX_HEALTH, PLAYER_STARTING_ARMOR);

        auto* inventory = gameObject->AddComponent<InventoryComponent>();
        inventory->AddItem(ItemType::Sword);
        inventory->SelectSlot(0);

        gameObject->AddComponent<StaminaComponent>();

        gameObject->AddComponent<HealthBarComponent>();

        gameObject->AddComponent<QuestHudComponent>();

        gameObject->AddComponent<InventoryHudComponent>();

        gameObject->AddComponent<PlayerMovementComponent>();

        gameObject->AddComponent<PlayerAnimationComponent>();

        gameObject->AddComponent<PlayerAttackComponent>();

        gameObject->AddComponent<PotionSpawnerComponent>();

        auto camera = gameObject->AddComponent<XYZEngine::CameraComponent>();

        camera->SetWindow(&XYZEngine::RenderSystem::Instance()->GetMainWindow());

        camera->SetBaseResolution(SCREEN_WIDTH, SCREEN_HEIGHT);
        camera->ZoomBy(0.85f);

        gameObject->GetComponent<XYZEngine::TransformComponent>()->SetWorldPosition(
            SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);

        LOG_INFO("Player created");
    }

    XYZEngine::GameObject* Player::GetGameObject()
    {
        return gameObject;
    }

    HealthComponent* Player::GetHealthComponent()
    {
        return gameObject->GetComponent<HealthComponent>();
    }

    void Player::Update(float deltaTime) {}

    void Player::SetEnemyContainer(EnemyContainer* container)
    {
        auto attack = gameObject->GetComponent<PlayerAttackComponent>();

        if (attack != nullptr)
        {
            attack->SetEnemyContainer(container);
        }

        auto* questHud = gameObject->GetComponent<QuestHudComponent>();
        if (questHud != nullptr)
        {
            questHud->SetEnemyContainer(container);
        }
    }
} // namespace XYZRoguelike
