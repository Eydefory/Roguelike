#pragma once

#include "Component.h"
#include "InventoryComponent.h"

namespace XYZRoguelike
{
    class ItemPickupComponent : public XYZEngine::Component
    {
    public:
        ItemPickupComponent(XYZEngine::GameObject* gameObject, XYZEngine::GameObject* player,
                            ItemType type);

        void Update(float deltaTime) override;
        void Render() override;

    private:
        XYZEngine::GameObject* player = nullptr;
        ItemType type = ItemType::Empty;
        float inventoryFullMessageTimer = 0.f;
    };
} // namespace XYZRoguelike
