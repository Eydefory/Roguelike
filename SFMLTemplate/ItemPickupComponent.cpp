#include "ItemPickupComponent.h"

#include "CameraComponent.h"
#include "Constants.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "HealthComponent.h"
#include "InventoryHudComponent.h"
#include "TransformComponent.h"

#include <algorithm>
#include <cmath>

namespace XYZRoguelike
{
    ItemPickupComponent::ItemPickupComponent(XYZEngine::GameObject* gameObject,
                                             XYZEngine::GameObject* player, ItemType type)
        : Component(gameObject), player(player), type(type)
    {
    }

    void ItemPickupComponent::Update(float deltaTime)
    {
        auto* hud = player != nullptr ? player->GetComponent<InventoryHudComponent>() : nullptr;
        if (player == nullptr || (hud != nullptr && hud->IsOpen()))
        {
            return;
        }

        auto* inventory = player->GetComponent<InventoryComponent>();
        auto* pickupTransform = gameObject->GetComponent<XYZEngine::TransformComponent>();
        auto* playerTransform = player->GetComponent<XYZEngine::TransformComponent>();
        if (inventory == nullptr || pickupTransform == nullptr || playerTransform == nullptr)
        {
            return;
        }

        const auto pickupPosition = pickupTransform->GetWorldPosition();
        const auto playerPosition = playerTransform->GetWorldPosition();
        const float dx = pickupPosition.x - playerPosition.x;
        const float dy = pickupPosition.y - playerPosition.y;
        if (dx * dx + dy * dy > ITEM_PICKUP_RANGE * ITEM_PICKUP_RANGE)
        {
            inventoryFullMessageTimer = std::max(0.f, inventoryFullMessageTimer - deltaTime);
            return;
        }

        if (!inventory->CanAddItem(type))
        {
            inventoryFullMessageTimer = std::max(0.f, inventoryFullMessageTimer - deltaTime);
            if (hud != nullptr && inventoryFullMessageTimer <= 0.f)
            {
                hud->ShowFeedback("Inventory full. Free a slot to pick this up.",
                                  sf::Color(255, 140, 120));
                inventoryFullMessageTimer = 2.f;
            }
            return;
        }

        if (!inventory->AddItem(type))
        {
            return;
        }

        if (hud != nullptr)
        {
            hud->ShowFeedback(std::string(GetItemDisplayName(type)) + " added to inventory.");
        }

        auto* camera = player->GetComponent<XYZEngine::CameraComponent>();
        if (camera != nullptr)
        {
            camera->Shake(0.08f, 2.5f);
        }

        XYZEngine::GameWorld::Instance()->DestroyGameObject(gameObject);
    }

    void ItemPickupComponent::Render() {}
} // namespace XYZRoguelike
