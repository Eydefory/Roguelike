#pragma once

#include "Component.h"
#include "Constants.h"

#include <array>
#include <cstddef>

namespace XYZRoguelike
{
    enum class ItemType
    {
        Empty,
        Sword,
        Bow,
        HealingPotion,
        ShieldPotion,
        StaminaPotion,
        Key
    };

    struct InventoryStack
    {
        ItemType type = ItemType::Empty;
        unsigned int quantity = 0;
    };

    using InventorySlots = std::array<InventoryStack, INVENTORY_SLOT_COUNT>;

    const char* GetItemTextureName(ItemType type);
    const char* GetItemDisplayName(ItemType type);

    class InventoryComponent : public XYZEngine::Component
    {
    public:
        explicit InventoryComponent(XYZEngine::GameObject* gameObject);

        void Update(float deltaTime) override;
        void Render() override;

        bool AddItem(ItemType type, unsigned int quantity = 1);
        ItemType AddRandomPotionDrop();
        bool CanAddItem(ItemType type, unsigned int quantity = 1) const;
        bool UseItem(std::size_t slotIndex);
        bool UseSelectedItem();
        bool ConsumeSelectedKey();
        bool SelectSlot(std::size_t slotIndex);
        bool HasItem(ItemType type) const;

        const InventorySlots& GetItems() const;
        std::size_t GetSelectedSlot() const;
        ItemType GetSelectedItem() const;

    private:
        bool ConsumeOne(std::size_t slotIndex);

        InventorySlots items{};
        std::size_t selectedSlot = 0;
    };
} // namespace XYZRoguelike
