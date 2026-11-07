#include "Wall.h"
#include "Constants.h"
#include "ResourceSystem.h"

#include "RigidbodyComponent.h"
#include "SpriteColliderComponent.h"
#include "SpriteRendererComponent.h"
#include "TransformComponent.h"

#include <array>
#include <random>

namespace XYZRoguelike
{
    Wall::Wall(float cellX, float cellY)
    {
        gameObject = XYZEngine::GameWorld::Instance()->CreateGameObject("Wall");

        auto renderer = gameObject->AddComponent<XYZEngine::SpriteRendererComponent>();

        static std::mt19937 randomEngine(std::random_device{}());
        static const std::array<const char*, 5> textureNames = {
            "wall_variant_1", "wall_variant_2", "wall_variant_3", "wall_variant_4",
            "wall_variant_5"};
        std::uniform_int_distribution<std::size_t> chooseTexture(0, textureNames.size() - 1);
        auto* resources = XYZEngine::ResourceSystem::Instance();
        const auto selectedIndex = chooseTexture(randomEngine);
        const sf::Texture* texture = nullptr;
        for (std::size_t offset = 0; offset < textureNames.size(); ++offset)
        {
            const auto index = (selectedIndex + offset) % textureNames.size();
            texture = resources->GetTextureShared(textureNames[index]);
            if (texture != nullptr)
            {
                break;
            }
        }

        if (texture != nullptr)
        {
            renderer->SetTexture(*texture);

            renderer->SetPixelSize(static_cast<int>(TILE_SIZE), static_cast<int>(TILE_SIZE));
        }

        auto transform = gameObject->GetComponent<XYZEngine::TransformComponent>();

        transform->SetWorldPosition(ROOM_OFFSET_X + cellX * TILE_SIZE + HALF_TILE,
                                    ROOM_OFFSET_Y + cellY * TILE_SIZE + HALF_TILE);

        gameObject->AddComponent<XYZEngine::SpriteColliderComponent>();

        auto body = gameObject->AddComponent<XYZEngine::RigidbodyComponent>();

        body->SetKinematic(true);
    }

    XYZEngine::GameObject* Wall::GetGameObject() const
    {
        return gameObject;
    }
} // namespace XYZRoguelike
