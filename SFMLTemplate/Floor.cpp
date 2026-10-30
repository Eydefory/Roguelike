#include "Floor.h"
#include "Constants.h"
#include "ResourceSystem.h"
#include "SpriteRendererComponent.h"
#include "TransformComponent.h"

namespace XYZRoguelike
{
    Floor::Floor(float x, float y, float size)
    {
        gameObject =
            XYZEngine::GameWorld::Instance()->CreateGameObject("FloorTile");

        auto* renderer =
            gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();

        const sf::Texture* texture =
            XYZEngine::ResourceSystem::Instance()->GetTextureShared("floor");

        if (texture != nullptr)
        {
            renderer->SetTexture(*texture);

            renderer->SetPixelSize(
                static_cast<int>(size),
                static_cast<int>(size)
            );
        }

        auto* transform =
            gameObject->GetComponent<XYZEngine::TransformComponent>();

        
        transform->SetWorldPosition(
            x + size * 0.5f,
            y + size * 0.5f
        );
    }

    XYZEngine::GameObject* Floor::GetGameObject()
    {
        return gameObject;
    }
}