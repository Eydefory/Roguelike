#include "Player.h"
#include "PlayerMovementComponent.h"
#include "Constants.h"

#include "RigidbodyComponent.h"

namespace XYZRoguelike
{
    Player::Player()
    {
        gameObject =
            XYZEngine::GameWorld::Instance()->CreateGameObject("Player");

        auto renderer =
            gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();

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

        
        auto camera =
            gameObject->AddComponent<XYZEngine::CameraComponent>();

        camera->SetWindow(
            &XYZEngine::RenderSystem::Instance()->GetMainWindow()
        );

        camera->SetBaseResolution(SCREEN_WIDTH, SCREEN_HEIGHT);

        gameObject->GetComponent<XYZEngine::TransformComponent>()
            ->SetWorldPosition(
                SCREEN_WIDTH / 2.f,
                SCREEN_HEIGHT / 2.f
            );
    }

    XYZEngine::GameObject* Player::GetGameObject()
    {
        return gameObject;
    }
}