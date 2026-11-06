
#include "Player.h"
#include "Constants.h"
#include "EnemyContainer.h"
#include "HealthBarComponent.h"
#include "Loger.h"
#include "PlayerAttackComponent.h"
#include "PlayerMovementComponent.h"
#include "RigidbodyComponent.h"

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
            renderer->SetPixelSize(PLAYER_WIDTH, PLAYER_HEIGHT);
        }

        gameObject->AddComponent<XYZEngine::SpriteColliderComponent>();

        gameObject->AddComponent<XYZEngine::RigidbodyComponent>();

        gameObject->AddComponent<XYZEngine::InputComponent>();

        gameObject->AddComponent<PlayerMovementComponent>();

        gameObject->AddComponent<PlayerAttackComponent>();

        auto health = gameObject->AddComponent<HealthComponent>();

        health->Initialize(100, 25);

        gameObject->AddComponent<HealthBarComponent>();

        auto camera = gameObject->AddComponent<XYZEngine::CameraComponent>();

        camera->SetWindow(&XYZEngine::RenderSystem::Instance()->GetMainWindow());

        camera->SetBaseResolution(SCREEN_WIDTH, SCREEN_HEIGHT);

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
            attack->SetEnemyContainer(container);
    }
} // namespace XYZRoguelike
