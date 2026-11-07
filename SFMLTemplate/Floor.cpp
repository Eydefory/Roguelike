#include "Floor.h"
#include "Constants.h"
#include "ResourceSystem.h"
#include "SpriteRendererComponent.h"
#include "TransformComponent.h"

#include <array>
#include <random>

namespace XYZRoguelike
{
    Floor::Floor(float x, float y, float size)
    {
        gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("FloorTile");

        auto* renderer = gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();

        static std::mt19937 randomEngine(std::random_device{}());
        static const std::array<const char*, 2> textureNames = {"floor_variant_1",
                                                                "floor_variant_2"};
        std::uniform_int_distribution<std::size_t> chooseTexture(0, textureNames.size() - 1);
        const sf::Texture* texture = XYZEngine::ResourceSystem::Instance()->GetTextureShared(
            textureNames[chooseTexture(randomEngine)]);

        if (texture != nullptr)
        {
            renderer->SetTexture(*texture);

            renderer->SetPixelSize(static_cast<int>(size), static_cast<int>(size));
        }

        auto* transform = gameObject->GetComponent<XYZEngine::TransformComponent>();

        transform->SetWorldPosition(x + size * 0.5f, y + size * 0.5f);
    }

    XYZEngine::GameObject* Floor::GetGameObject()
    {
        return gameObject;
    }
} // namespace XYZRoguelike
