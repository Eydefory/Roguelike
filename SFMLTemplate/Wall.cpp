#include "Wall.h"
#include "Constants.h"
#include "ResourceSystem.h"

#include "SpriteRendererComponent.h"
#include "SpriteColliderComponent.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"

namespace XYZRoguelike
{
    Wall::Wall(float cellX, float cellY, const std::string& textureName)
    {
        gameObject =
            XYZEngine::GameWorld::Instance()->CreateGameObject("Wall");

        auto renderer =
            gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();

        const sf::Texture* texture =
            XYZEngine::ResourceSystem::Instance()->GetTextureShared(textureName);

        if (texture != nullptr)
        {
            renderer->SetTexture(*texture);

            renderer->SetPixelSize(
                static_cast<int>(TILE_SIZE),
                static_cast<int>(TILE_SIZE)
            );
        }

        auto transform =
            gameObject->GetComponent<XYZEngine::TransformComponent>();

        transform->SetWorldPosition(
            ROOM_OFFSET_X + cellX * TILE_SIZE + HALF_TILE,
            ROOM_OFFSET_Y + cellY * TILE_SIZE + HALF_TILE
        );

        
        gameObject->AddComponent<XYZEngine::SpriteColliderComponent>();

        
        auto body =
            gameObject->AddComponent<XYZEngine::RigidbodyComponent>();

        body->SetKinematic(true);
    }

    XYZEngine::GameObject* Wall::GetGameObject() const
    {
        return gameObject;
    }
}